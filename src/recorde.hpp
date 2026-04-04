#ifndef RECORDE_HPP
#define RECORDE_HPP

#include <fstream>
#include <iostream>

class Recorde{
    public:
    static double ler(){
        std::ifstream arquivo("recorde.txt");
        double recorde =0.0;
        if(arquivo.is_open()){
        
        arquivo >> recorde;
        arquivo.close();
        }

        return recorde;
    }
    static void salvar(double novoRecorde) {
        std::ofstream arquivo("recorde.txt");
        
        if (arquivo.is_open()) {
            arquivo << novoRecorde;
            arquivo.close();
        } else {
            std::cerr << "Erro ao salvar o recorde em recorde.txt" << std::endl;
        }
    }
};

#endif