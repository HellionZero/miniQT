#!/bin/bash
# Script para baixar e compilar SDL2 localmente no projeto MiniQT

set -e

SDL2_VERSION="2.28.5"
SDL2_URL="https://github.com/libsdl-org/SDL/releases/download/release-${SDL2_VERSION}/SDL2-${SDL2_VERSION}.tar.gz"
SDL2_DIR="lib/SDL2"
BUILD_DIR="${SDL2_DIR}/build"
INSTALL_DIR="${SDL2_DIR}/install"

echo "=========================================="
echo "  Configurando SDL2 Local para MiniQT"
echo "=========================================="
echo ""

# Criar diretórios
mkdir -p "${SDL2_DIR}"
cd "${SDL2_DIR}"

# Baixar SDL2 se necessário
if [ ! -f "SDL2-${SDL2_VERSION}.tar.gz" ]; then
    echo "[1/5] Baixando SDL2 ${SDL2_VERSION}..."
    wget -q --show-progress "${SDL2_URL}" || curl -L -o "SDL2-${SDL2_VERSION}.tar.gz" "${SDL2_URL}"
else
    echo "[1/5] SDL2 ${SDL2_VERSION} já baixado"
fi

# Extrair
if [ ! -d "SDL2-${SDL2_VERSION}" ]; then
    echo "[2/5] Extraindo SDL2..."
    tar -xzf "SDL2-${SDL2_VERSION}.tar.gz"
else
    echo "[2/5] SDL2 já extraído"
fi

# Configurar
echo "[3/5] Configurando SDL2..."
cd "SDL2-${SDL2_VERSION}"
mkdir -p build
cd build

# Configurar com CMake ou autotools
if command -v cmake &> /dev/null; then
    cmake .. \
        -DCMAKE_INSTALL_PREFIX="../../../install" \
        -DCMAKE_BUILD_TYPE=Release \
        -DSDL_SHARED=OFF \
        -DSDL_STATIC=ON \
        -DSDL_TEST=OFF \
        -DSDL_RENDER_ENABLED_BY_DEFAULT=ON
else
    ../configure --prefix="$(pwd)/../../../install" \
        --enable-static \
        --disable-shared \
        --disable-video-wayland
fi

# Compilar
echo "[4/5] Compilando SDL2 (isso pode demorar)..."
make -j$(nproc)

# Instalar localmente
echo "[5/5] Instalando SDL2 localmente..."
make install

cd ../../../..

echo ""
echo "=========================================="
echo "  SDL2 compilado com sucesso!"
echo "=========================================="
echo ""
echo "Arquivos instalados em:"
echo "  Headers: ${INSTALL_DIR}/include/"
echo "  Libs: ${INSTALL_DIR}/lib/"
echo ""
echo "Para usar, compile com SDL2=1:"
echo "  make SDL2=1"
echo ""
