#include <iostream>
#include <string>
#include <random>

#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem> //C++ 17

#include <vector>
#include <unordered_map>

class Cliente
{
private:
	char apellido[30]{};
	char nombre[30]{};
	char DNI[9];
public:
	static auto generarDNIAleatorio() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> rand_digits(0, 9);

		std::stringstream st;

		for (unsigned i = 0; i < 8; i++) {
			st << rand_digits(gen);
		}

		return st.str();
	}
	static auto generarClientesAleatorios(const unsigned& cantidad) {
		std::ifstream archivo_lista_apellidos("lista_apellidos.txt", std::ios::in);
		std::ifstream archivo_lista_nombres("lista_nombres.txt", std::ios::in);

		if (!archivo_lista_apellidos || !archivo_lista_nombres) {
			std::cerr << "No se puedo abrir el archivo\n";
			exit(EXIT_FAILURE);
		}

		std::string linea;
		std::vector<std::string> lista_apellidos, lista_nombres;
		std::vector<Cliente> lista_clientes;

		while (std::getline(archivo_lista_apellidos, linea)) {
			lista_apellidos.push_back(linea);
		}
		while (std::getline(archivo_lista_nombres, linea)) {
			lista_nombres.push_back(linea);
		}

		archivo_lista_apellidos.close();
		archivo_lista_nombres.close();

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> pos_aleatoria_1(0, static_cast<int>(lista_apellidos.size()) - 1);
		std::uniform_int_distribution<> pos_aleatoria_2(0, static_cast<int>(lista_nombres.size()) - 1);

		while (lista_clientes.size() < cantidad) {
			lista_clientes.push_back(Cliente(generarDNIAleatorio(), lista_apellidos[pos_aleatoria_1(gen)], lista_nombres[pos_aleatoria_2(gen)]));
		}

		return lista_clientes;
	}
public:
	Cliente(const std::string& DNI, const std::string& apellido, const std::string& nombre) {
		setDNI(DNI);
		setApellido(apellido);
		setNombre(nombre);
	}

	Cliente() : DNI(""), apellido(""), nombre("") {}

	~Cliente() {}

	std::string getApellido() const {
		return apellido;
	}

	std::string getNombre() const {
		return nombre;
	}

	std::string getDNI() const {
		return DNI;
	}

	void setApellido(const std::string& apellido) {

		int longitud = static_cast<int>(apellido.length());

		longitud = (longitud < 30 ? longitud : 30 - 1);

		apellido.copy(this->apellido, longitud);

		this->apellido[longitud] = '\0';
		// adjunta un carácter nulo a apellido
	}

	void setNombre(const std::string& nombre) {
		int longitud = static_cast<int>(nombre.length());
		
		longitud = (longitud < 30 ? longitud : 30 - 1);

		nombre.copy(this->nombre, longitud);

		this->nombre[longitud] = '\0';
		// adjunta un carácter nulo a nombre
	}

	void setDNI(const std::string& DNI) {
		if (DNI.length() != 8) {
			throw std::invalid_argument("El DNI debe contener 8 caracteres!\n");
		}
		else {
			DNI.copy(this->DNI, 8);
			this->DNI[8] = '\0';
		}
	}

public:
	
	friend std::ostream& operator<<(std::ostream& os, const Cliente& c1) {
		return os << std::left << c1.getDNI() << std::setw(15) << c1.getApellido() << std::setw(15) << c1.getNombre();
	}
};

template<typename T>
class GestorAlmacenamientoFijo {

private:
	unsigned num_total_pags{};
	unsigned campos_por_pag{};
	unsigned pag_actual{};
	size_t PAGE_SIZE{};
	std::string nombre_archivo;
	std::fstream archivo;
	std::unordered_map<unsigned, size_t>pos_paginas;
private:
	template<typename U>
	class Encapsular {
		template<typename R> friend class GestorAlmacenamientoFijo;
	private:
		unsigned id{ 1 };
		U dato;
	public:
		Encapsular(const U& dato, const unsigned& id) : dato(dato), id(id) {}
		Encapsular() {}
		~Encapsular() {}
	};
public:
	GestorAlmacenamientoFijo(const std::string& nombre_archivo, const unsigned& num_total_pags, const unsigned& campos_por_pag) {

		this->nombre_archivo = nombre_archivo;
		this->campos_por_pag = campos_por_pag;
		this->num_total_pags = num_total_pags;
		this->pag_actual = 0u;
		this->PAGE_SIZE = sizeof(Encapsular<T>) * num_total_pags * campos_por_pag;

		Encapsular<T> campo;

		archivo.open(nombre_archivo, std::ios::in | std::ios::binary);

		if (!archivo) { //Si el archivo no existe, crea uno
			archivo.close();
			archivo.open(nombre_archivo, std::ios::out | std::ios::binary);

			for (unsigned j = 1; j <= campos_por_pag * num_total_pags; ++j) {
				campo.id = j;
				archivo.write(reinterpret_cast<const char*>(&campo), sizeof(Encapsular<T>));
			}
		}
		else { //Si existe el archivo, compruba que su cantidad de bytes sea igual que a la multiplicacion de
			//cantidad de paginas, cantidad campos y el peso del tipo de dato que almacena (PAGE_SIZE)		
			std::filesystem::path direccion_archivo(nombre_archivo);
			auto peso_archivo{ std::filesystem::file_size(nombre_archivo) };
			if (peso_archivo != PAGE_SIZE) {
				std::cerr << "La cantidad de bytes del archivo abierto, no coincide con la cantidad que deberia tener!\n\n"
					<< "CANTIDAD DE PAGINAS: " << num_total_pags << "\nCAMPOS POR PAGINA: " << campos_por_pag << "\nBYTES DEL TIPO DE DATO: " << sizeof(Encapsular<T>)
					<< "\nBYTES POR PAGINA: " << campos_por_pag * sizeof(Encapsular<T>) << "\nBYTES DEL ARCHIVO INGRESADO: " << peso_archivo << "\nBYTES QUE DEBERIA TENER: " << PAGE_SIZE << '\n';
				exit(EXIT_FAILURE);
			}
		}

		for (unsigned i = 0; i < num_total_pags + 1; i++) {
			pos_paginas.insert(std::make_pair(i, i * campos_por_pag * sizeof(Encapsular<T>)));
		}
		archivo.close();
	}

	~GestorAlmacenamientoFijo() {}

	auto getPAGE_SIZE() const {
		return PAGE_SIZE;
	}

	std::streampos getActualPagina() {
		return static_cast<std::streampos>(pos_paginas[pag_actual]);
	}

	void cambiarPaginaActual(const unsigned& pag_actual) {
		if (pag_actual >= num_total_pags) {
			throw std::invalid_argument("El archivo no contiene la cantidad de paginas indica!\n");
		}
		else {
			this->pag_actual = pag_actual;
		}
	}

	void leerBloque(const unsigned& pag_actual)   {

		if (pag_actual >= num_total_pags) {
			throw std::invalid_argument("El archivo no contiene la cantidad de paginas indicada!\n");
		}

		archivo.open(nombre_archivo, std::ios::in | std::ios::binary);

		if (!archivo) {
			std::cerr << "No se pudo abrir el archivo '" + nombre_archivo + "'\n";
			exit(EXIT_FAILURE);
		}

		archivo.seekg(pos_paginas[pag_actual]);

		std::streampos pos_limite{ static_cast<std::streampos>(pos_paginas[pag_actual + 1]) };

		std::cout << std::left << std::setw(15) << "ID" << std::setw(15) << "DNI" << std::setw(15) << "APELLIDO" << std::setw(7) << "NOMBRE\n";

		Encapsular<T> campo;

		while (archivo.tellg() < pos_limite && !archivo.eof())
		{
			archivo.read(reinterpret_cast<char*>(&campo), sizeof(Encapsular<T>));
			std::cout << std::left << std::setw(15) << campo.id << std::setw(15) << campo.dato << '\n';
		}

		archivo.close();
	}

	void leerPrimerBloque() {
		leerBloque(0);
	}

	void leerUltimoBloque() {
		leerBloque(num_total_pags - 1);
	}

	void leerAnteriorBloque() {
		leerBloque(pag_actual - 1);
	}

	void leerSiguienteBloque() {
		leerBloque(pag_actual + 1);
	}

	void leerBloqueActual() {
		leerBloque(pag_actual);
	}

	void leerTodo() {

		archivo.open(nombre_archivo, std::ios::in | std::ios::binary);

		if (!archivo) {
			std::cerr << "No se pudo abrir el archivo '" + nombre_archivo + "'\n";
			exit(EXIT_FAILURE);
		}

		std::cout << std::left << std::setw(15) << "ID" << std::setw(15) << "DNI" << std::setw(15) << "APELLIDO" << std::setw(7) << "NOMBRE\n";

		Encapsular<T> campo;

		while (archivo && !archivo.eof())
		{
			archivo.read(reinterpret_cast<char*>(&campo), sizeof(Encapsular<T>));
			std::cout << std::left << std::setw(15) << campo.id << std::setw(15) << campo.dato << '\n';
		}

		archivo.close();
	}

	void escribirBloque(const unsigned& pag_actual, const std::vector<T>& registro) {

		if (pag_actual >= num_total_pags) {
			throw std::invalid_argument("El archivo no contiene la cantidad de paginas indica!\n");
		}

		archivo.open(nombre_archivo, std::ios::in | std::ios::out | std::ios::binary);
	
		if (!archivo) {
			std::cerr << "No se pudo abrir el archivo '" + nombre_archivo + "'!\n";
			exit(EXIT_FAILURE);
		}
	
		std::vector<Encapsular<T>> registro_encapsulado;

		unsigned id = 1 + campos_por_pag * pag_actual;
		for (const auto& r : registro) {
			registro_encapsulado.push_back(Encapsular<T>(r, id));
			id++;
		}

		unsigned i = 0;
		for (const auto& re : registro_encapsulado) {

			archivo.seekp(pos_paginas[pag_actual] + i * sizeof(Encapsular<T>)); //Ubicar la posicion del puntero

			archivo.write(reinterpret_cast<const char*>(&re), sizeof(Encapsular<T>)); //Escribe en el archivo

			i++;
		}

		archivo.close();
	}

	void escribirBloqueActual(const std::vector<T>& registro) {
		escribirBloque(pag_actual);
	}

	void agregarBloqueVacio() {

		archivo.open(nombre_archivo, std::ios::ate | std::ios::out | std::ios::in | std::ios::binary);

		if (!archivo) {
			std::cerr << "No se pudo abrir el archivo '" + nombre_archivo + "'\n";
			exit(EXIT_FAILURE);
		}

		auto campos_totales = num_total_pags * campos_por_pag;

		Encapsular<T> campo;

		for (unsigned j = 1; j <= campos_por_pag; ++j) {
			campo.id = campos_totales + j;
			archivo.write(reinterpret_cast<const char*>(&campo), sizeof(Encapsular<T>));
		}

		num_total_pags += 1;
		PAGE_SIZE = num_total_pags * campos_por_pag * sizeof(Encapsular<T>);
		pos_paginas.insert(std::make_pair(num_total_pags, PAGE_SIZE));


		archivo.close();
	}
};


int main() {

	//Lista de apellidos para generacion aleatoria de registros 
	//https://drive.google.com/file/d/1HmPXRtjAusgkiNEQRhZUfiH1N1iTwOa7/view?usp=sharing

	//Lista de nombres para generacion aleatoria de registros 
	//https://drive.google.com/file/d/1pm6MlsY6xh-RXvd_jDwHfGA2MMmzdnTw/view?usp=sharing

	auto clientes = GestorAlmacenamientoFijo<Cliente>("prueba.bin", 5, 50); //Nombre del archivo, numeros de paginas, cantidad de campos por pagina
																			//Pude cargar archivos ya creados, para leer o añadir contenido
	try {
		clientes.escribirBloque(0, Cliente::generarClientesAleatorios(50)); 

		clientes.escribirBloque(1, Cliente::generarClientesAleatorios(50));

		clientes.escribirBloque(2, Cliente::generarClientesAleatorios(50));

		clientes.escribirBloque(3, Cliente::generarClientesAleatorios(50));

		clientes.escribirBloque(4, Cliente::generarClientesAleatorios(50));

		clientes.escribirBloque(7, Cliente::generarClientesAleatorios(50)); //Intentar registros a una pagina que sale del rango
	}
	catch (const std::invalid_argument& e) {
		std::cout << e.what() << '\n';
	}

	clientes.cambiarPaginaActual(2); 

	std::cout << "\n\n***BLOQUE ACTUAL***\n";
	clientes.leerBloqueActual();

	std::cout << "\n\n***SIGUIENTE BLOQUE***\n";
	clientes.leerSiguienteBloque();

	std::cout << "\n\n***ANTERIOR BLOQUE***\n";
	clientes.leerAnteriorBloque();

	std::cout << "\n\n***PRIMER BLOQUE***\n";
	clientes.leerPrimerBloque();

	clientes.agregarBloqueVacio();	//Añade un bloque vacio, modifica la cantidad de pag del archivo, 
									//Al volver a abrir el archivo desde el constructor se debe cambiar 
									//la cantidad de paginas a la nueva cantidad o enviara un error
	std::cout << "\n\n***NUEVO BLOQUE***\n";
	clientes.leerBloque(5); //Nuevo Bloque

	clientes.leerTodo();

	return 0;
}
