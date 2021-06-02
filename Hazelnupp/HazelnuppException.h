#pragma once
#include <stdexcept>

class HazelnutException : public std::exception {};
class HazelnutConstraintMissmatch : public HazelnutException {};
