#ifndef STRUKTURY_HPP_
#define STRUKTURY_HPP_

//Standardowe nagłówki C/C++
#include <cstdint>

//Nagłówki z katalogu programu
#include "TypyWyliczeniowe.hpp"

struct Rozkaz
{
	enum STANY_MASZYNY stan;
	uint32_t czas;
};

typedef struct Rozkaz Rozkaz;

#endif
