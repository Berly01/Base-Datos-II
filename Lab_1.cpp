//Necesita C++ 17
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
        ~Nodo() { std::cout << "Se destruye\n"; };
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
    std::shared_ptr<Nodo> raiz{ std::make_shared<Nodo>(' ') };

    //Retorna una tupla, un shared_ptr<Nodo> que apunta al ultimo nodo siguiendo la llave dada, a su vez
    //que se va eliminando cada caracter de la llave que se recorre, tambien retorna esta llave final
    auto buscar_nodo(const std::shared_ptr<Nodo>& nodo_inicial, string llave, bool vec_con_caracter = false) {
        
        if (vec_con_caracter) {
            return std::make_tuple(nodo_inicial, llave);
        }

        auto nodo_final{ nodo_inicial };
        auto llave_final{ llave };

        for (const auto& n : nodo_inicial->hijos) {
            if (n->caracter == llave[0]) {
                llave.erase(llave.begin());            
                auto [nf, lf] = buscar_nodo(n, llave);
                nodo_final = nf;
                llave_final = lf;
                break;
            }
        }
        return buscar_nodo(nodo_final, llave_final, true);
    }

    auto buscar_nodo_sin_hijos(const std::shared_ptr<Nodo>& nodo_inicial, string llave, bool finaliza = false) {
        
        if (finaliza) {
            return std::make_tuple(nodo_inicial, llave);
        }

        auto nodo_final{ nodo_inicial };
        auto llave_final{ llave };

        for (const auto& i : nodo_inicial->hijos) {

            if (i->caracter == llave[0]) {
                llave.erase(llave.begin());  
                auto [nf, lf] = buscar_nodo_sin_hijos(i, llave);

                if (nf->hijos.size() > 1) {
                    nodo_final = nf;
                    llave_final = lf;
                }
                break;
            }
        }

        return buscar_nodo_sin_hijos(nodo_final, llave_final, true);
    }

    //Usando la funcion buscar_nodo, que busca y retorna el ultimo nodo que coinsida con los caracteres
    //de la llave brindada, se crean nodos con los caracteres faltantes en la llave y se inserta al nodo final
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
            throw std::logic_error("¡El arbol esta vacio!");
        }

        else {

            auto [nodo_final, llave_final] = buscar_nodo(nodo_inicial, llave);
            //buscar_nodo busca el nodo usando la llave ingresada, y por cada caracter quen coinsida con los que 
            //que almacenan los nodos se va eliminando la que contiene la llave.
            //Si la llave que retorna buscar_nodo esta vacia, quiere decir que existe un clave con la llave ingresada
            if (llave_final.empty()) { 

                for (const auto& i : nodo_final->hijos) {
                    if (i->bandera) {
                        return static_cast<Hoja<T>*>(i.get());
                    }
                }
            }

            else {
                Hoja<T>* h = nullptr;
                return h;
            }
        }
    }

   
    void eliminar_nodo(std::shared_ptr<Nodo> nodo_inicial, string llave) {
  
        for (auto i : nodo_inicial->hijos) {

            if (i->caracter == llave[0]) {
                //std::cout << i->caracter << '\n';
                llave.erase(llave.begin());
                eliminar_nodo(i, llave);
                i.reset();
                //std:: cout << (i.get())->caracter ;
                break;
            }

            else if (i->bandera && llave.empty()) {
                i.reset();
                //std::cout << static_cast<Hoja<T>*>(i.get())->valor;
            }
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
            throw std::invalid_argument("¡La llave ingresada no coincide con ningun elemento en el arbol!");
        }
        else {
            return hoja->valor;
        }
    }

    void eliminar_valor(string llave) {

        auto [nodo_final, llave_final] = buscar_nodo_sin_hijos(raiz, llave);

        eliminar_nodo(nodo_final, llave_final);
    }

    const void prueba(string llave) {

        /*
        for (const auto& i : raiz->hijos) {
            std::cout << i->caracter << '\n';
        }
        */
    }
};

int main() {

    auto arbol = ArbolDigital<int>();

    arbol.insertar_elemento("o", 45); 

    arbol.insertar_elemento("oso", 25);

    arbol.insertar_elemento("osos", 25321);

    arbol.insertar_elemento("ave", 15);

    arbol.eliminar_valor("osos");

    try {
        std::cout << arbol.obtener_valor("o") << "\n";

        std::cout << arbol.obtener_valor("oso") << "\n";

        std::cout << arbol.obtener_valor("osos") << "\n";

        std::cout << arbol.obtener_valor("ave") << "\n";

    }
    catch (const std::invalid_argument& e) {
        std:: cout << e.what() << '\n';
    }

    catch (const std::logic_error& e) {
        std::cout << e.what() << '\n';
    }
   

    return 0;
}
