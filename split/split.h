#ifndef SPLIT_HPP
#define SPLIT_HPP
#include <algorithm>
#include <string>
#include <vector>

using std::reverse;
using std::vector;
using std::string;

   vector<string> dividir(string str, char delimiter = '-'){

		vector<string> ret;
    	if(str.empty()){
    	//push_back(x) -> insere x no fim de vetor
        ret.push_back(string(""));
        return ret;
   		}

	    unsigned i = 0;
	    string strstack;
	    while(!(str.empty()) && (str[0] == delimiter)){
		//elimina elemento referenciado de inicio a fim
		str.erase(0,1);
		}
		//recoloca o tamanho do vetor caso primeiro elemento for ' '
	    reverse(str.begin(), str.end());

	    while(!(str.empty()) && (str[0] == delimiter)){
		str.erase(0,1);
		}
	    reverse(str.begin(), str.end());

	    while(!str.empty())
	    {
	        ret.push_back(str.substr(i, str.find(delimiter)));
	        str.erase(0,str.find(delimiter));
	        while(!(str.empty()) && (str[0] == delimiter)) {str.erase(0,1);}
	    }

	    return ret;
	    /*
		Forma de testar
		for(int i = 0; i < split(frase).size(); i++)
    			cout << split(frase)[i] << endl;
				*/
}
#endif // STR_SEPARAR_HPP
