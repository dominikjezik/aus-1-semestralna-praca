#include "Console.h"

void Console::clear()
{
	system("cls");
}

void Console::input(std::string& userInput)
{
	getline(std::cin, userInput);
}

void Console::mainMenu()
{
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "[1] Filtrovanie uzemnych jednotiek" << std::endl;
	std::cout << "[2] Navigacia v uzemnych jednotkach" << std::endl;
	std::cout << "[3] Vyhladavanie informacii podla nazvu" << std::endl;
	std::cout << "[x] Ukoncit program" << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "Zadajte volbu: ";
}

void Console::hierarchyMenu(Iterator& iterator)
{
	std::cout << "----------------------------------------" << std::endl;
	std::cout << ">> " << (*iterator)->official_title() << std::endl;
	
	if (iterator.canGoToParent())
	{
		(*iterator)->printDetails();
		std::cout << "----------------------------------------" << std::endl;
		std::cout << " 0 Presun sa na otca" << std::endl;
	}
	else
	{
		auto* root = dynamic_cast<AggregatedDataUnit*>(*iterator);
		root->printAggregatedData();
		std::cout << "----------------------------------------" << std::endl;
	}

	int i = 1;

	auto begin = iterator.beginSons();
	auto end = iterator.endSons();
	while (begin != end)
	{
		std::cout << "   - " << i++ << " " << (*begin)->data_->official_title() << std::endl;
		++begin;
	}

	std::cout << "----------------------------------------" << std::endl;
	std::cout << " * Spusti algoritmus nad aktualnou urovnou " << std::endl;
	std::cout << " x Vrat sa do hlavneho menu" << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << " Zadajte volbu: ";
}

void Console::filterPredicatesMenu()
{
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "Vyh�ad�vanie pod�a: " << std::endl
		<< "[1] �zemn� jednotka obsahuje dan� v�raz (default)" << std::endl
		<< "[2] �zemn� jednotka zac�na na dan� v�raz" << std::endl
		<< "[3] �zemn� jednotka je dan�ho typu" << std::endl
		<< "[4] Minim�lna po�adovan� hustota obyvate�stva" << std::endl
		<< "[5] Maxim�lna po�adovan� hustota obyvate�stva" << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "Zadajte volbu: ";
}

void Console::filterAskForSearchArgument(std::string predicateNumber)
{
	if (predicateNumber == "3")
	{
		std::cout << "Zadajte typ uzemnej jednotky (obec, okres, kraj): ";
	}
	else if (predicateNumber == "4")
	{
		std::cout << "Zadajte minimalnu pozadovanu hustotu obyvatelstva: ";
	}
	else if (predicateNumber == "4")
	{
		std::cout << "Zadajte maximalnu pozadovanu hustotu obyvatelstva: ";
	}
	else
	{
		std::cout << "Zadajte v�raz pre vyh�ad�vanie: ";
	}
}

void Console::printSearchResultsKrajeTitle()
{
	std::cout << "----------------- Kraje ----------------" << std::endl;
}

void Console::printSearchResultsOkresyTitle()
{
	std::cout << "---------------- Okresy ----------------" << std::endl;
}

void Console::printSearchResultsObceTitle()
{
	std::cout << "----------------- Obce -----------------" << std::endl;
}

void Console::printSearchResultsGeneralTitle()
{
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "V�sledky vyh�ad�vania: " << std::endl;
}

void Console::noResults()
{
	std::cout << "Pre zadan� predikat a argument sa nena�li �iadne �zemn� jednotky." << std::endl;
}

void Console::waitForUserInput()
{
	std::cout << "----------------------------------------" << std::endl;
	std::cout << " x Vr�ti� sa sp�" << std::endl;
	std::cin.get();
}

void Console::selectTable()
{
	std::cout << "Zadajte z ktorej tabulky chcete vyhladavat: (kraje, okresy, obce)" << std::endl;
}

void Console::enterKey()
{
	std::cout << "Zadajte n�zov �zemnej jednotky (k���):" << std::endl;
}

void Console::noResultKraj()
{
	std::cout << "Nena�iel sa �iadny kraj s dan�m n�zvom." << std::endl;
}

void Console::noResultOkres()
{
	std::cout << "Nena�iel sa �iadny okres s dan�m n�zvom." << std::endl;
}

void Console::noResultObec()
{
	std::cout << "Nena�la sa �iadna obec s dan�m n�zvom." << std::endl;
}

void Console::waitForUserInputAskForSorting()
{
	std::cout << "----------------------------------------" << std::endl;
	std::cout << " & Utried vysledky" << std::endl;
	std::cout << " x Vrat sa spat" << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << " Zadajte volbu: ";
}

void Console::sortingComparatorsMenu()
{
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "Zoradit uzemne jednotky: " << std::endl
		<< "[1] Abecedne (default)" << std::endl
		<< "[2] Podla poctu samohlasok" << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "Zadajte volbu: ";
}

