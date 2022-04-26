#include "tinyXML/tinyxml2.cpp"
#include "tinyXML/tinyxml2.h"
#include <iostream>
#include <list>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;
using namespace tinyxml2;

void printSVG(tinyxml2::XMLDocument &doc, tinyxml2::XMLPrinter &printer)
{
	// Print in memory
	doc.Print(&printer);
	cout << printer.CStr() << endl;
}
template <typename T>
void printList(list<T> &list){
	for (auto v : list)
        cout << v << "\n";
}

template <typename T>
bool contains(list<T> & listOfElements, const T & element)
{
    // Find the iterator if element in list
    auto it = std::find(listOfElements.begin(), listOfElements.end(), element);
    //return if iterator points to end or not. It points to end then it means element
    // does not exists in list
    return it != listOfElements.end();
}

bool Test(list<string> lista,XMLDocument &xml_doc)
{



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
				auto *pAttr = pPath->FindAttribute("d");
				auto *pColor = pPath->FindAttribute("fill");
				//cout << pColor->Value() << endl;
				cout << pAttr->Value() << endl;
				if (pColor)
				{
					if (contains(lista,string(pColor->Value())))
					{
						cout << "Color concuerda" << endl;
					}	
				}
				pPath = pPath->NextSiblingElement("path");
			}
		}
	}
	return true;
}
void sizeBox(XMLDocument &doc,string &sizeX,string &sizeY){
	auto pRoot = doc.FirstChildElement("svg");
	string view= pRoot->FindAttribute("viewBox")->Value();
	view.erase(0, view.find(' ')+1);
	view.erase(0, view.find(' ')+1);
	sizeX=view.substr(0, view.find(' '));
	view.erase(0, view.find(' ')+1);
	sizeY=view.substr(0, view.find(' '));


}
int calculated_B(int x, int y){
	return y+(-1*x);

}
list<float> calculated(int x, int y,float angulo,int frames,int sizeX, int sizeY){
	//formula=y=tan(angulo)x+b

	//y-x=b
	list <float> result;
	int b=calculated_B(x,y);
	while (x<sizeX){
		x+=frames;
		float y=(tan(angulo)*(x)+b);
		if(y>sizeY)
		break;
		result.push_back(y);

	}
	return result;


}

int router(list<string> lista,XMLDocument &doc,float angulo,int frames){
	int x=7;
	int y=5;
	string sizeX;
	string sizeY;
	sizeBox(doc,sizeX,sizeY);
	//cout<<sizeX<<endl;
	//cout<<sizeY<<endl;
	list<float> listY= calculated(7,5,angulo,frames,stoi(sizeX),stoi(sizeY));
	printList(listY);
	return 1;

}
int main()
{
	cout<<acos(-1)<<endl
	list<string> lista={"#3FBF85","#EF87BB"};
	tinyxml2::XMLDocument doc;
	doc.LoadFile("arch_svg/ejemplo.svg");
	//Test(lista,xml_doc);
	router(lista,doc,M_PI/4,10);


}