#!/bin/bash
# Script para testar todos os exemplos da MiniQT

echo "======================================"
echo "  Testando Exemplos MiniQT"
echo "======================================"
echo ""

# Cores
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

# Função para testar um executável
test_executable() {
    local name=$1
    local path=$2
    
    echo -n "[$name] "
    
    if [ ! -f "$path" ]; then
        echo -e "${RED}✗ Executável não encontrado${NC}"
        return 1
    fi
    
    if [ ! -x "$path" ]; then
        echo -e "${RED}✗ Não é executável${NC}"
        return 1
    fi
    
    echo -e "${GREEN}✓ Compilado${NC}"
    
    # Mostrar informações
    echo "  Tipo: $(file -b "$path" | cut -d',' -f1)"
    echo "  Tamanho: $(du -h "$path" | cut -f1)"
    
    return 0
}

# Testar cada exemplo
echo "1. Basic Demo"
test_executable "basic_demo" "./basic_demo/basic_demo"
echo ""

echo "2. Widget Demo"
test_executable "widget_demo" "./widget_demo/widget_demo"
echo ""

echo "3. Simple App"
test_executable "simple_app" "./simple_app/simple_app"
echo ""

echo "======================================"
echo "  Instruções de Uso"
echo "======================================"
echo ""
echo "Para executar um exemplo:"
echo "  cd basic_demo && ./basic_demo"
echo "  cd widget_demo && ./widget_demo"
echo "  cd simple_app && ./simple_app"
echo ""
echo "Controles:"
echo "  ESC - Sair"
echo "  Mouse - Interagir (widget_demo)"
echo ""
