// Deklaracja funkcji preprocesora
#pragma once
#include <string>

// Wczytuje plik i uruchamia preprocesor C (cpp -P).
// Zwraca wynikowy tekst jako std::string lub wyrzuca std::runtime_error przy błędzie.
std::string RunPreprocessor(const std::string &filename);
