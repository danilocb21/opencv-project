# 🎮 Jogo de Fuga com OpenCV

Projeto/jogo desenvolvido em C++ com OpenCV, cujo objetivo é fugir do inimigo pelo maior tempo possível.
O controle do jogador é feito pela detecção de rosto via webcam, aplicando conceitos de Orientação a Objetos vistos na disciplina.

## 📋 Sobre o Projeto

Este trabalho foi desenvolvido na disciplina de Linguagem de Programação I e combina visão computacional com lógica de jogo em tempo real.
Durante a partida, o jogador perde vidas ao colidir com o inimigo, e o sistema registra o melhor tempo sobrevivido em arquivo.

## 🎓 Informações Acadêmicas

- **Instituição:** Universidade Federal da Paraíba (UFPB)
- **Curso:** Engenharia da Computação
- **Disciplina:** Linguagem de Programação I
- **Professor:** Derzu Omaia
- **Período:** 2025.2

## 👥 Autores

- [Danilo Beuttenmuller](https://github.com/danilocb21)
- [Eduardo Augusto](https://github.com/Edu92337)
- [Rafael Ferreira](https://github.com/rafael01-e)

## 🚀 Características Técnicas

- **Linguagem:** C++23
- **Paradigma:** Programação Orientada a Objetos (POO)
- **Bibliotecas principais:** OpenCV 4 (core, imgproc, highgui, videoio, objdetect)
- **Entrada:** Webcam (detecção de rosto com Haar Cascade)
- **Persistência:** Armazenamento de recorde em arquivo texto
- **Compilação:** Makefile + pkg-config

### Conceitos de POO Aplicados

- **Encapsulamento:** Organização dos componentes do jogo em classes específicas
- **Abstração:** Separação de responsabilidades (jogo, tempo, áudio, texto, jogador e inimigo)
- **Composição:** Classe principal agregando componentes de estado e renderização
- **Reuso de código:** Utilitários e entidades reaproveitáveis durante o loop do jogo

## 📊 Diagrama de Classes

## ⚙️ Funcionalidades

1. **Detecção de jogador por rosto** em tempo real via webcam.
2. **Movimentação automática do inimigo** perseguindo a posição do jogador.
3. **Sistema de vidas** com invulnerabilidade temporária após dano.
4. **HUD em tempo real** exibindo tempo vivo, vidas e recorde.
5. **Persistência de recorde** no arquivo `src/recorde.txt`.
6. **Feedback sonoro** para dano e fim de jogo.
7. **Menu inicial** para iniciar partida, exibir recorde e encerrar.

## 🛠️ Como Compilar e Executar

### Pré-requisitos

- Linux com acesso a webcam
- Compilador C++ com suporte a C++23 (g++ recomendado)
- OpenCV 4 instalado
- pkg-config
- make
- (Opcional, para áudio) ao menos um player instalado: `mpg123`, `ffplay`, `paplay` ou `cvlc`

### Instalação de dependências (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install -y g++ make pkg-config libopencv-dev mpg123
```

### Compilação

```bash
make
```

### Execução

```bash
./app
```

Ou:

```bash
make run
```

### Limpeza dos arquivos compilados

```bash
make clean
```

## 🎯 Como Jogar

1. Execute o programa e escolha **1. Iniciar jogo** no menu.
2. Posicione seu rosto em frente a webcam para controlar o jogador.
3. Evite o inimigo pelo maior tempo possível.
4. Pressione `Q`/`q` ou `ESC` para sair da janela do jogo.
5. No menu, use **2. Exibir recorde** para consultar o melhor tempo.

## 📁 Estrutura do Projeto

```
opencv-project/
├── assets/
│   ├── FAH-with-shotgun.mp3
│   ├── fahh_sound_effect.mp3
│   ├── haarcascade_frontalface_default.xml
│   └── inimigo.png
├── build/
├── src/
│   ├── game.cpp
│   ├── game.hpp
│   ├── inimigo.hpp
│   ├── jogador.hpp
│   ├── main.cpp
│   ├── recorde.hpp
│   ├── recorde.txt
│   ├── som.hpp
│   ├── tempo.hpp
│   ├── texto.hpp
│   └── utils.hpp
├── app
├── makefile
└── README.md
```

### Descrição dos Componentes

- **assets/**: Arquivos de imagem, áudio e cascade para detecção facial
- **src/**: Código-fonte principal do jogo e classes auxiliares
- **src/recorde.txt**: Persistência do maior tempo sobrevivido
- **makefile**: Regras de compilação e execução

## 🔧 Detalhes de Implementação

### Classes Principais

- **Game:** Controla ciclo principal, estados, colisão, HUD e encerramento
- **Jogador:** Detecta rosto e define posição do jogador no frame
- **Inimigo:** Renderiza e move o inimigo em direção ao jogador
- **Tempo:** Mede e formata tempo de sobrevivência
- **Recorde:** Lê e salva recorde em arquivo
- **Som:** Dispara efeitos sonoros com players do sistema
- **Texto:** Desenha textos na interface com OpenCV

## 💾 Persistência de Dados

O jogo utiliza o arquivo `src/recorde.txt` para armazenar o melhor tempo.

- O recorde é carregado no início da partida.
- Se um novo recorde for atingido, ele é salvo automaticamente.

## 📝 Licença

Este projeto foi desenvolvido para fins acadêmicos.