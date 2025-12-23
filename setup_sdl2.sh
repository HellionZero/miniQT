#!/bin/bash
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    setup_sdl2.sh                                      :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#              #
#    Updated: 2025/12/23 00:00:00 by lsarraci         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Script para baixar e compilar SDL2 localmente no projeto

set -e

SDL2_VERSION="2.28.5"
SDL2_URL="https://github.com/libsdl-org/SDL/releases/download/release-${SDL2_VERSION}/SDL2-${SDL2_VERSION}.tar.gz"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LIB_DIR="${SCRIPT_DIR}/lib"
SDL2_DIR="${LIB_DIR}/SDL2"

echo "======================================"
echo "  MiniQT - SDL2 Local Setup"
echo "======================================"
echo ""
echo "This script will:"
echo "  1. Download SDL2 ${SDL2_VERSION} source code"
echo "  2. Compile it as a static library"
echo "  3. Install it locally in: ${SDL2_DIR}"
echo ""

# Criar diretório de bibliotecas se não existir
mkdir -p "${LIB_DIR}"
cd "${LIB_DIR}"

# Download SDL2
if [ ! -f "SDL2-${SDL2_VERSION}.tar.gz" ]; then
    echo "[1/5] Downloading SDL2 source code..."
    curl -L "${SDL2_URL}" -o "SDL2-${SDL2_VERSION}.tar.gz"
    echo "✓ Download complete"
else
    echo "[1/5] SDL2 source already downloaded"
fi

# Extract
if [ ! -d "SDL2-${SDL2_VERSION}" ]; then
    echo "[2/5] Extracting SDL2..."
    tar -xzf "SDL2-${SDL2_VERSION}.tar.gz"
    echo "✓ Extraction complete"
else
    echo "[2/5] SDL2 already extracted"
fi

# Configure and build
cd "SDL2-${SDL2_VERSION}"

if [ ! -f "Makefile" ]; then
    echo "[3/5] Configuring SDL2 build..."
    ./configure \
        --prefix="${SDL2_DIR}" \
        --enable-static \
        --disable-shared \
        --disable-video-wayland \
        --disable-video-vulkan \
        --disable-render-metal
    echo "✓ Configuration complete"
else
    echo "[3/5] SDL2 already configured"
fi

echo "[4/5] Building SDL2 (this may take a few minutes)..."
make -j$(nproc)
echo "✓ Build complete"

echo "[5/5] Installing SDL2 locally..."
make install
echo "✓ Installation complete"

echo ""
echo "======================================"
echo "  SDL2 Setup Complete!"
echo "======================================"
echo ""
echo "SDL2 installed at: ${SDL2_DIR}"
echo ""
echo "Next steps:"
echo "  1. Run 'make' in the project root"
echo "  2. The build system will automatically use SDL2"
echo ""
echo "Files created:"
echo "  - ${SDL2_DIR}/include/SDL2/  (headers)"
echo "  - ${SDL2_DIR}/lib/           (static libraries)"
echo ""
