# SDL2 Local Installation

Este diretório conterá a instalação local da SDL2.

## Setup Automático

Execute o script de setup na raiz do projeto:

```bash
cd /nfs/homes/lsarraci/my_shell/miniQT
chmod +x setup_sdl2.sh
./setup_sdl2.sh
```

## Setup Manual

Se preferir fazer manualmente:

```bash
# 1. Baixar SDL2
cd lib
wget https://github.com/libsdl-org/SDL/releases/download/release-2.28.5/SDL2-2.28.5.tar.gz
tar -xzf SDL2-2.28.5.tar.gz
cd SDL2-2.28.5

# 2. Configurar
./configure --prefix=$(pwd)/../SDL2 --enable-static --disable-shared

# 3. Compilar e instalar
make -j$(nproc)
make install
```

## Estrutura após instalação

```
SDL2/
├── include/
│   └── SDL2/
│       ├── SDL.h
│       ├── SDL_video.h
│       ├── SDL_events.h
│       └── ... (outros headers)
├── lib/
│   ├── libSDL2.a       (biblioteca estática)
│   └── pkgconfig/
└── bin/
    └── sdl2-config
```

## Compilação do Projeto

O Makefile automaticamente detectará SDL2 se estiver neste diretório.

## Remover SDL2

```bash
rm -rf lib/SDL2
rm -rf lib/SDL2-2.28.5*
```
