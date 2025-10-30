#!/bin/bash

# SIC/XE Assembler - Build Setup Script
# This script creates the build directory and compiles the assembler

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  SIC/XE Assembler - Build Setup${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# Get script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"

# Step 1: Create build directory
echo -e "${YELLOW}[1/4]${NC} Creating build directory..."
if [ -d "$BUILD_DIR" ]; then
    echo -e "      ${GREEN}✓${NC} Build directory already exists"
else
    mkdir -p "$BUILD_DIR"
    echo -e "      ${GREEN}✓${NC} Build directory created"
fi
echo ""

# Step 2: Configure with CMake
echo -e "${YELLOW}[2/4]${NC} Configuring project with CMake..."
cd "$BUILD_DIR"
if cmake .. ; then
    echo -e "      ${GREEN}✓${NC} CMake configuration successful"
else
    echo -e "      ${RED}✗${NC} CMake configuration failed"
    exit 1
fi
echo ""

# Step 3: Build the project
echo -e "${YELLOW}[3/4]${NC} Building the assembler..."
if make ; then
    echo -e "      ${GREEN}✓${NC} Build successful"
else
    echo -e "      ${RED}✗${NC} Build failed"
    exit 1
fi
echo ""

# Step 4: Verify executable
echo -e "${YELLOW}[4/4]${NC} Verifying executable..."
EXECUTABLE="$BUILD_DIR/sicxe-assembler"
if [ -f "$EXECUTABLE" ]; then
    echo -e "      ${GREEN}✓${NC} Executable created successfully"
    echo ""
    echo -e "${GREEN}========================================${NC}"
    echo -e "${GREEN}  Build Complete!${NC}"
    echo -e "${GREEN}========================================${NC}"
    echo ""
    echo -e "Executable location: ${BLUE}$EXECUTABLE${NC}"
    echo ""
    echo -e "Usage:"
    echo -e "  ${BLUE}$EXECUTABLE${NC} <input.asm> <output.obj>"
    echo ""
    echo -e "Example:"
    echo -e "  ${BLUE}$EXECUTABLE${NC} test.asm output.obj"
    echo ""
else
    echo -e "      ${RED}✗${NC} Executable not found"
    exit 1
fi
