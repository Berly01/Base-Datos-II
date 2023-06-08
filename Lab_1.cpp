//C++ 17
#include <iostream>
#include <memory>
#include <vector>
#include <tuple>
using std::string;

template<typename T>
class ArbolDigital {

    class Nodo {
        template<typename T> friend class ArbolDigital;
    private:
        std::vector<std::shared_ptr<Nodo>> hijos;
        char caracter{};
        bool bandera{};
    public:
        Nodo(char caracter, bool bandera = false) : caracter(caracter), bandera(bandera) { }
        ~Nodo() {};
    };

    template<typename T>
    class Hoja : public Nodo {
        template<typename T> friend class ArbolDigital;
    private:
        T valor{};
    public:
        Hoja(T valor) : Nodo(' ', true), valor(valor) {};
        ~Hoja() {};
    };

private:
    std::shared_ptr<Nodo> raiz{ std::make_shared<Nodo>('\0') };

    auto buscar_nodo(const std::shared_ptr<Nodo>& nodo_inicial, string llave, bool vec_con_caracter = false) {

        if (nodo_inicial->hijos.empty() || vec_con_caracter) {
            return std::make_tuple(nodo_inicial, llave);
        }

        auto nodo_final{ nodo_inicial };

        for (const auto& n : nodo_inicial->hijos) {
            if (n->caracter == llave[0]) {
                llave.erase(llave.begin());            
                nodo_final = std::get<std::shared_ptr<Nodo>>(buscar_nodo(n, llave));
                break;
            }
        }

        return buscar_nodo(nodo_final, llave, true);
    }

    void insertar_elemento(std::shared_ptr<Nodo> nodo_inicial, string llave, const T& valor) {

        auto [nodo_final, llave_final] = buscar_nodo(nodo_inicial, llave);
        auto nodo_raiz{ nodo_final };

        nodo_raiz->hijos.push_back(std::make_shared<Nodo>(llave_final[0]));
        auto nodo_aux{ nodo_raiz->hijos.back() };

        for (int i = 1; i < llave_final.length(); i++) {
            nodo_aux->hijos.push_back(std::make_shared<Nodo>(llave_final[i]));
            nodo_aux = nodo_aux->hijos.back();
        }

        nodo_aux->hijos.push_back(std::make_shared<Hoja<T>>(valor));
    }

    auto obtener_valor(const std::shared_ptr<Nodo>& nodo_inicial, string llave) {

        if (raiz->hijos.empty()) {
            throw std::logic_error("El arbol esta vacio!");
        }

        else {

            auto nodo_final = std::get<std::shared_ptr<Nodo>>(buscar_nodo(nodo_inicial, llave));

            if (!nodo_final->hijos.empty() && !(nodo_final->hijos.back()->bandera)) {
                Hoja<T>* h = nullptr;
                return h;
            }
            else {
                return static_cast<Hoja<T>*>(nodo_final->hijos.back().get());
            }
        }
    }

    void eliminar_elemento(std::shared_ptr<Nodo> nodo_inicial, string llave) {

        auto [nodo_final, llave_final] = buscar_nodo(nodo_final, llave);

        if (nodo_final->hijos.size() == 1 && nodo_final->hijos.back()->bandera) {

        }
    }

public:
    ArbolDigital() {};
    ~ArbolDigital() {};

    void insertar_elemento(string llave, T valor) {
        insertar_elemento(raiz, llave, valor);
    }

    auto obtener_valor(string llave) {

        auto hoja = obtener_valor(raiz, llave);

        if (hoja == nullptr) {
            throw std::invalid_argument("La llave ingresada no coincide con ningun elemento en el arbol!");
        }
        else {
            return hoja->valor;
        }
    }

    void eliminar_valor(string llave) {

    }

    const void imprimir_arbol() {
        for (const auto& i : raiz->hijos) {
            std::cout << i->caracter << " ";
        }
    }
};

int main() {

    auto arbol = ArbolDigital<int>();

    arbol.insertar_elemento("oso", 6565346);

    arbol.insertar_elemento("orto", 543634);

    arbol.insertar_elemento("mierda", 43123);

    std::cout << arbol.obtener_valor("orto") << " " << true;

    return 0;
}
