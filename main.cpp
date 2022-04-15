#include "tinyXML/tinyxml2.cpp"
#include <iostream>
using namespace std;

void printSVG(tinyxml2::XMLDocument &doc, tinyxml2::XMLPrinter &printer) {
	// Print in memory
	doc.Print(&printer);
	cout << printer.CStr() << endl;
}
int main() {

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLPrinter printer;
	
	doc.LoadFile("ejemplo.svg");

	// Save the original file
	doc.SaveFile("wifi-1.svg");

	// Search for the 'svg' element
	tinyxml2::XMLElement *svg_Element = doc.FirstChildElement();
	tinyxml2::XMLElement *g_Element = svg_Element->FirstChildElement();

	// Create and insert a new element
	tinyxml2::XMLElement *newPath_Element = g_Element->InsertNewChildElement("path");
	newPath_Element->SetAttribute("d", "M200 0 L125 200 L275 200 Z");
	newPath_Element->SetAttribute("fill", "#04aa6d");

	// Save the file with one new element
	doc.SaveFile("wifi-2.svg");

	// Create and insert a new element
	newPath_Element = g_Element->InsertNewChildElement("path");
	newPath_Element->SetAttribute("d", "M250 0 L175 200 L325 200 Z");
	newPath_Element->SetAttribute("fill", "#fcba03");

	// Save the file with the last new element
	doc.SaveFile("wifi-3.svg");


	printSVG(doc, printer);
	return 0;
}