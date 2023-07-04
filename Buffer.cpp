#include <iostream>
#include <string>
#include <string_view> //C++ 17

#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem> //C++ 17
#include <bitset>

#include <vector>
#include <unordered_map>

/*
class Prueba {
public:
	std::streampos pos_sig{};
	double data{};
	std::vector<long> vec{};
public:
	Prueba() {}
};

Prueba p1;

p1.pos_sig = 10000;

p1.data = 4.3543243;

for (auto i = 10000l; i < 10010l; i += 1) {
	p1.vec.push_back(i);
}

std::fstream archivo("archivo.bin", std::ios::out | std::ios::binary);

archivo.write(reinterpret_cast<const char*>(&p1.pos_sig), sizeof(p1.pos_sig));

archivo.write(reinterpret_cast<const char*>(&p1.data), sizeof(p1.data));

for (const auto i : p1.vec) {
	archivo.write(reinterpret_cast<const char*>(&i), sizeof(i));
}

archivo.close();

Prueba p2;

archivo.open("archivo.bin", std::ios::in | std::ios::binary);

archivo.read(reinterpret_cast<char*>(&p2.pos_sig), sizeof(p2.pos_sig));
archivo.read(reinterpret_cast<char*>(&p2.data), sizeof(p2.data));

long aux{ 0 };
for (auto i = 0; i < p1.vec.size(); i++) {
	archivo.read(reinterpret_cast<char*>(&aux), sizeof(aux));
	p2.vec.push_back(aux);
}

archivo.close();
*/

void error() {
	std::vector<bool> vec_1(10, 0);

	const auto tam = sizeof(std::vector<bool>) + sizeof(bool) * vec_1.size();

	std::fstream archivo("archivo.bin", std::ios::out | std::ios::binary);

	archivo.write(reinterpret_cast<const char*>(&vec_1), tam);

	archivo.close();

	archivo.open("archivo.bin", std::ios::in, std::ios::binary);

	std::vector<bool> vec_2{};

	archivo.read(reinterpret_cast<char*>(&vec_2), tam);

	for (const auto& i : vec_2) {
		std::cout << i << " ";
	}

	archivo.close();
}

class MapaBits {
private:
	std::vector<bool> bits;
public:
	MapaBits(const size_t& cantidad) {
		for (unsigned i = 0; i < cantidad; i++) {
			bits.push_back(false);
		}
	}
	void inicializarNuevoMapa(const size_t& cantidad) {
		if (!bits.empty()) {
			bits.clear();
		}
		for (unsigned i = 0; i < cantidad; i++) {
			bits.push_back(false);
		}
	}
	void setBit(const size_t& indice) {
		if (indice >= bits.size()) {
			throw std::out_of_range("El indice ingresado sale del rango del mapa de bits!\n");
		}
		bits[indice] = true;
	}
	void clearBit(const size_t& indice) {
		if (indice >= bits.size()) {
			throw std::out_of_range("El indice ingresado sale del rango del mapa de bits!\n");
		}
		bits[indice] = false;
	}
	bool getBit(const size_t& indice) const {
		if (indice >= bits.size()) {
			throw std::out_of_range("El indice ingresado sale del rango del mapa de bits!\n");
		}
		return bits[indice];
	}
	auto getTotalSize() {
		return sizeof(MapaBits) + sizeof(bool) * bits.size();
	}
};

class PersonTitanic
{
private:
	int passenger_Id{};
	bool survived{};
	int P_class;
	char sex[7]{};
	int age{};
	int sib_Sp{};
	int parch{};
	char embarked{};
	double fare{};
	char ticket[25]{};
	char cabin[15]{};
	char name[80]{};
public:
	PersonTitanic() {
		passenger_Id, P_class, age, sib_Sp, parch = -1;
		fare = -1.0;
		survived = false;
		embarked = '\0';
	}
	void setPassenger_Id(const int& passenger_Id) { this->passenger_Id = passenger_Id; }
	void setSurvived(bool survived) { this->survived = survived; }
	void setP_class(const int& P_class) { this->P_class = P_class; }
	void setName(const std::string_view& name) {
		auto longitud = name.length();
		longitud = longitud < 80 ? longitud : 80 - 1;
		name.copy(this->name, longitud);
		this->name[longitud] = '\0';
	}
	void setSex(const std::string_view& sex) {

		if (sex == "male") {
			sex.copy(this->sex, 4);
			this->sex[4] = '\0';
		}
		else if (sex == "female") {
			sex.copy(this->sex, 6);
			this->sex[6] = '\0';
		}
	}
	void setAge(const int& age) { this->age = age; }
	void setSib_Sp(const int& sib_Sp) { this->sib_Sp = sib_Sp; }
	void setParch(const int& parch) { this->parch = parch; }
	void setFare(const double& fare) { this->fare = fare; }
	void setTicket(const std::string_view& ticket) {
		auto longitud = ticket.length();
		longitud = longitud < 25 ? longitud : 25 - 1;
		ticket.copy(this->ticket, longitud);
		this->ticket[longitud] = '\0';
	}
	void setCabin(const std::string_view& cabin) {
		auto longitud = cabin.length();
		longitud = longitud < 25 ? longitud : 15 - 1;
		cabin.copy(this->cabin, longitud);
		this->ticket[longitud] = '\0';
	}
	void setEmbarked(const char& embarked) { this->embarked = embarked; }
public:
	static auto getRegistroCompleto() {

		std::ifstream archivo("titanic.csv", std::ios::in);

		if (!archivo) {
			exit(EXIT_FAILURE);
		}

		std::string linea{};
		std::stringstream ss{};
		std::string campo{};
		std::vector<std::vector<std::string>> vec_registros{};
		std::vector<std::string> vec_nombres_campos{};

		std::getline(archivo, linea);
		ss << linea;

		while (std::getline(ss, campo, ',')) {
			vec_nombres_campos.push_back(campo);
		}
		ss.clear();

		while (std::getline(archivo, linea)) {

			ss << linea;
			std::vector<std::string> vec_campos{};

			while (std::getline(ss, campo, ',')) {
				vec_campos.push_back(campo);
			}

			vec_campos[3].erase(vec_campos[3].begin()); //Borrar el '"' inicial del campo name
			vec_campos[4].erase(vec_campos[4].end() - 1); //Borrar el '"' final del campo name

			if (vec_campos.size() == 12) { //En el campo 'Embarked' hay campos que estan vacias y al estar al final no se lee el vacio
				vec_campos.push_back("");  //y vec_campos solo alamcena 12 valores y no 13, provocando el error de out_range al acceder a estos datos
			}

			//ss.str("");
			ss.clear();
			vec_registros.push_back(vec_campos);
		}

		archivo.close();
		return std::make_tuple(vec_nombres_campos, vec_registros);
	}
	static auto getTodasPersonasTitanic() {

		auto vec_registros = std::get<1>(getRegistroCompleto());

		std::vector<PersonTitanic> vec_personas{};

		for (const auto& vec_campos : vec_registros) {
			auto persona = PersonTitanic();
			persona.setPassenger_Id(vec_campos[0].empty() ? -1 : std::stoi(vec_campos[0]));
			persona.setSurvived(vec_campos[1].empty() ? 0 : std::stoi(vec_campos[1]));
			persona.setP_class(vec_campos[2].empty() ? -1 : std::stoi(vec_campos[2]));
			persona.setName(std::string_view(vec_campos[3] + ", " + vec_campos[4]));
			persona.setSex(vec_campos[5]);
			persona.setAge(vec_campos[6].empty() ? -1 : std::stoi(vec_campos[6]));
			persona.setSib_Sp(vec_campos[7].empty() ? -1 : std::stoi(vec_campos[7]));
			persona.setParch(vec_campos[8].empty() ? -1 : std::stoi(vec_campos[8]));
			persona.setTicket(vec_campos[9]);
			persona.setFare(vec_campos[10].empty() ? -1.0 : std::stod(vec_campos[10]));
			persona.setCabin(vec_campos[11]);
			persona.setEmbarked(vec_campos[12][0]);
			vec_personas.push_back(persona);
		}

		return vec_personas;
	}
	static auto getPersonasTitanic(const unsigned& rango_i, const unsigned& rango_j) {
		if (rango_i > rango_j) {
			throw std::invalid_argument("Argumento Invalido\n");
		}
		if (rango_i > 890 || rango_j > 890) {
			throw std::out_of_range("Fuera de Rango\n");
		}
		auto personas = getTodasPersonasTitanic();
		std::vector<PersonTitanic> personas_selec{};

		personas_selec.assign(personas.cbegin() + rango_i, personas.cbegin() + rango_j);

		return personas_selec;
	}

public:
	friend std::ostream& operator<<(std::ostream& os, const PersonTitanic& p) {
		return os << std::left << std::setw(6) << p.passenger_Id << std::setw(6) << p.survived << std::setw(6)
			<< p.P_class << std::setw(60) << p.name << std::setw(16) << p.sex << std::setw(12)
			<< p.age << std::setw(6) << p.sib_Sp << std::setw(6) << p.parch << std::setw(20)
			<< p.ticket << std::setw(12) << p.fare << std::setw(15) << p.cabin << std::setw(6)
			<< p.embarked;
	}
};

template<typename T>
class ArchivoFijo {
private:
	class CabezeraInicial {
		template<typename R> friend class ArchivoFijo;
	private:
		unsigned num_total_pags{};
		unsigned campos_por_pag{};
	public:
		CabezeraInicial(const unsigned& num_total_pags, const unsigned& campos_por_pag) : num_total_pags(num_total_pags), campos_por_pag(campos_por_pag) {}
		CabezeraInicial() : num_total_pags(0), campos_por_pag(0) {}
	};

	class CabezeraPagina {
		template<typename R> friend class ArchivoFijo;
	private:
		std::streampos sig_cabezera_pagina{};
		size_t espacio_libre{};
	public:
		CabezeraPagina(const std::streampos& sig_cabezera_pagina) : sig_cabezera_pagina(sig_cabezera_pagina){}
		CabezeraPagina() : sig_cabezera_pagina(0), espacio_libre(0){}
	};

	template<typename U>
	class Encapsular {
		template<typename R> friend class ArchivoFijo;
	private:
		unsigned id{ 1 };
		U dato;
	public:
		Encapsular(const U& dato, const unsigned& id) : dato(dato), id(id) {}
		Encapsular() {}
		~Encapsular() {}
	};

private:
	unsigned num_total_pags{};
	unsigned campos_por_pag{};
	unsigned pag_actual{};
	size_t PAGE_SIZE{};
	std::string nombre_archivo;
	std::fstream archivo;
	std::unordered_map<unsigned, std::streampos> map_pos_cabezales{};
public:
	ArchivoFijo(const std::string_view& nombre_archivo, const unsigned& num_total_pags, const unsigned& campos_por_pag) {

		this->num_total_pags = num_total_pags;
		this->campos_por_pag = campos_por_pag;
		this->nombre_archivo = nombre_archivo;
		this->pag_actual = 0;

		const auto tam_CabezaraPagina = sizeof(CabezeraPagina);
		const auto tam_Encapsular = sizeof(Encapsular<T>);

		this->PAGE_SIZE = tam_Encapsular * campos_por_pag + tam_CabezaraPagina;

		archivo.open(nombre_archivo, std::ios::out | std::ios::binary);

		if (!archivo) {
			std::cerr << "No se pudo abrir el archivo '" + this->nombre_archivo + "'!\n";
			exit(EXIT_FAILURE);
		}

		auto cabezera = CabezeraInicial(num_total_pags, campos_por_pag);

		archivo.write(reinterpret_cast<const char*>(&cabezera), sizeof(CabezeraInicial));

		auto pos_actual = static_cast<size_t>(archivo.tellp());

		Encapsular<T> campo_vacio;

		for (unsigned i = 0; i < num_total_pags; i++) {

			CabezeraPagina cabezera_pagina(pos_actual + (i + 1) * PAGE_SIZE);

			map_pos_cabezales.insert(std::make_pair(i, pos_actual + i * PAGE_SIZE));

			archivo.write(reinterpret_cast<const char*>(&cabezera_pagina), tam_CabezaraPagina);

			for (unsigned e = 1; e <= campos_por_pag; e++) {
				campo_vacio.id = e;
				archivo.write(reinterpret_cast<const char*>(&campo_vacio), tam_Encapsular);
			}
		}

		archivo.close();
	}

	ArchivoFijo(const std::string_view& nombre_archivo) {

		std::filesystem::path direccion_archivo(nombre_archivo);
		if (!std::filesystem::exists(direccion_archivo)) {
			throw std::invalid_argument("La direccion del archivo ingresado no es valida!\n");
		}

		this->nombre_archivo = nombre_archivo;
		this->pag_actual = 0;

		archivo.open(nombre_archivo, std::ios::in, std::ios::binary);

		if (!archivo) {
			std::cerr << "No se pudo abrir el archivo '" + this->nombre_archivo + "'!\n";
			exit(EXIT_FAILURE);
		}

		CabezeraInicial cabezera;

		archivo.read(reinterpret_cast<char*>(&cabezera), sizeof(CabezeraInicial));

		this->num_total_pags = cabezera.num_total_pags;
		this->campos_por_pag = cabezera.campos_por_pag;

		map_pos_cabezales.insert(std::make_pair(0, sizeof(CabezeraInicial)));

		for (unsigned i = 1; i < num_total_pags; i++) {
			CabezeraPagina cabezera_pagina;

			archivo.read(reinterpret_cast<char*>(&cabezera_pagina), sizeof(CabezeraPagina));

			map_pos_cabezales.insert(std::make_pair(i, cabezera_pagina.sig_cabezera_pagina));

			archivo.seekg(cabezera_pagina.sig_cabezera_pagina);
		}
	 
		archivo.close();	
	}

	~ArchivoFijo() {}

	auto getPAGE_SIZE() const {
		return PAGE_SIZE;
	}

	auto getActualPagina() const {
		return pag_actual;
	}

	void setPaginaActual(const unsigned& pag_actual) {
		if (pag_actual >= num_total_pags) {
			throw std::invalid_argument("El archivo no contiene la cantidad de paginas indica!\n");
		}
		else {
			this->pag_actual = pag_actual;
		}
	}

	auto getNumPaginas() const {
		return num_total_pags;
	}

	auto getPosCabezales() const {
		return map_pos_cabezales;
	}

	void leerBloque(const unsigned& pag_actual) {

		if (pag_actual >= num_total_pags) {
			throw std::invalid_argument("El archivo no contiene la cantidad de paginas indicada!\n");
		}

		archivo.open(nombre_archivo, std::ios::in | std::ios::binary);

		if (!archivo) {
			std::cerr << "No se pudo abrir el archivo '" + nombre_archivo + "'\n";
			exit(EXIT_FAILURE);
		}

		archivo.seekg(static_cast<size_t>(map_pos_cabezales[pag_actual]) + sizeof(CabezeraPagina));

		std::streampos pos_limite{ map_pos_cabezales[pag_actual + 1] };

		Encapsular<T> campo;

		while (archivo.tellg() < pos_limite && !archivo.eof())
		{
			archivo.read(reinterpret_cast<char*>(&campo), sizeof(Encapsular<T>));
			std::cout << std::left << std::setw(15) << campo.dato << '\n';
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

		archivo.seekg(map_pos_cabezales[0] + sizeof(CabezeraPagina));

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

		archivo.seekp(static_cast<size_t>(map_pos_cabezales[pag_actual]) + sizeof(CabezeraPagina)); //Ubicar la posicion del puntero

		for (const auto& re : registro_encapsulado) {
			archivo.write(reinterpret_cast<const char*>(&re), sizeof(Encapsular<T>)); //Escribe en el archivo
		}

		archivo.close();
	}

	void escribirBloqueActual(const std::vector<T>& registro) {
		escribirBloque(pag_actual, registro);
	}

	/*
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
		pos_paginas.insert(std::make_pair(num_total_pags, PAGE_SIZE * num_total_pags));

		archivo.close();
	}
	*/

	void infoArchivo() const {
		std::cout << "CANTIDAD DE PAGINAS: " << num_total_pags << "\nCAMPOS POR PAGINA: " << campos_por_pag << "\nBYTES DEL TIPO DE DATO: " << sizeof(Encapsular<T>)
			<< "\nBYTES POR PAGINA: " << PAGE_SIZE << "\nBYTES TOTALES DEL ARCHIVO: " << PAGE_SIZE * num_total_pags * sizeof(Encapsular<T>) << '\n';
	}
};

template<typename T>
class BufferPoolManager {
private:
	class Page {
	private:
		bool dirty_bit{}, pinning{};
		unsigned pin_count{}, count{};
		unsigned page_id{};
		std::streampos pos_pagina{};
	public:
		Page(const unsigned& page_id, const std::streampos& pos_pagina) : page_id(page_id), pos_pagina(pos_pagina){
			dirty_bit, pinning = false;
			pin_count, count = 0;
		}

		Page() : page_id(0), pos_pagina(0) {
			dirty_bit, pinning = false;
			pin_count, count = 0;
		}

		~Page(){}
		friend std::ostream& operator<<(std::ostream& os, const Page& p) {
			return os << p.page_id;
		}
	};
private:
	const unsigned SIZE_FRAMES{};
	ArchivoFijo<T>* archivo{};
	std::unordered_map<unsigned, std::streampos> map_pages{};
	std::unordered_map<char, Page*> frame_pag{};
public:
	BufferPoolManager(ArchivoFijo<T>* archivo, const unsigned& SIZE_FRAMES) : SIZE_FRAMES(SIZE_FRAMES), archivo(archivo) {
		auto pos_cabezales = archivo->getPosCabezales();

		for (auto i = 0u; i < archivo->getNumPaginas(); i++) {
			map_pages.insert(std::make_pair(i, pos_cabezales[i]));
		}
		Page* page_null = nullptr;
		for (char i = 65; i < 65 + SIZE_FRAMES; i++) {
			frame_pag.insert(std::make_pair(i, page_null));
		}
	}

	~BufferPoolManager() {
		for (auto& i : frame_pag) {
			if (i.second != nullptr) {
				delete i.second;
			}
		}
	}

	auto fetchPage(const unsigned& page_id) const{

		for (const auto& i : frame_pag) {
			if (i.first == page_id) {
				return *i.second;
			}
		}
		throw std::invalid_argument("No se encontro la pagina indicada\n");
	}

	void newPage(const unsigned& page_id) {

		if (page_id > archivo->getNumPaginas()) {
			throw std::invalid_argument("No hay ninguna pagina con la id ingresada\n");
		}

		return;
	}

	void unPinPage(const unsigned& page_id) {

	}

	auto FlushPage(const unsigned& page_id) {
		return true;
	}

	void flushAllPages() {

	}

	void deletePage(const unsigned& page_id) {

	}

	friend std::ostream& operator<<(std::ostream& os, const BufferPoolManager& buffer) {
		for (const auto& b : buffer.frame_pag) {
			os << std::left << std::setw(6) << b.first;
			if (b.second == nullptr) 
				os << "NULL\n";
			else 
				os << *b.second << '\n';
		}
		return os;
	}
};


int main() {

	auto archivo = ArchivoFijo<PersonTitanic>("prueba.bin");

	auto buffer = BufferPoolManager<PersonTitanic>(&archivo, 4);


	buffer.newPage(2);

	std::cout << buffer;


	return 0;
}

