#ifndef RECORDE_HPP
#define RECORDE_HPP

#include <fstream>
#include <iostream>

class Recorde{
    public:
    static double ler(){
        // Atualizado para procurar na pasta src
        std::ifstream arquivo("src/recorde.txt"); 
        double recorde = 0.0;
        if(arquivo.is_open()){
            arquivo >> recorde;
            arquivo.close();
        }
        return recorde;
    }
    
    static void salvar(double novoRecorde) {
        // Atualizado para salvar na pasta src
        std::ofstream arquivo("src/recorde.txt"); 
        
        if (arquivo.is_open()) {
            arquivo << novoRecorde;
            arquivo.close();
        } else {
            std::cerr << "Erro ao salvar o recorde em src/recorde.txt" << std::endl;
        }
    }
};


#endif