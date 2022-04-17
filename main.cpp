#include "tinyXML/tinyxml2.cpp"
#include "tinyXML/tinyxml2.h"
#include <iostream>
using namespace std;
using namespace tinyxml2;

void printSVG(tinyxml2::XMLDocument &doc, tinyxml2::XMLPrinter &printer)
{
	// Print in memory
	doc.Print(&printer);
	cout << printer.CStr() << endl;
}

bool Test()
{
	tinyxml2::XMLDocument xml_doc;

	tinyxml2::XMLError eResult = xml_doc.LoadFile("arch_svg/example.svg");

	if (eResult != tinyxml2::XML_SUCCESS)
		return false;

	tinyxml2::XMLNode *root = xml_doc.FirstChildElement("svg");

	cout << "LEER ATRIBUTOS" << endl;

	auto pRoot = xml_doc.FirstChildElement("svg");
	if (pRoot)
	{
		auto pParm = pRoot->FirstChildElement("g");
		auto pPath = pParm->FirstChildElement("path");
		if (pPath)
		{
			while (pPath)
			{
				auto *pAttr = pPath->Attribute("d");
				{
					cout<<pAttr<<endl;
				}
				pPath = pPath->NextSiblingElement("path");
			}
		}
	}
}
int main()
{

	Test();
	// printSVG(doc, printer);
}