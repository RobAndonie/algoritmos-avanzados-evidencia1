// 1- Códigos están o no en transmisiones
// 2- Cantidad de incidencias de cada código en transmisiones
// 3- Posiciones donde se encontraron
// 4- Subsecuencia del código con un caracter menos del original que fue mas 
//		encontrado dentro de los tres archivos de transmisión

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

string readFile(const string& fileName) {
	ifstream file(fileName);
	string transmission, line;
	while (getline(file, line)) {
		transmission += line;
	}

	file.close();
	return transmission;
}

void readCodes(const string& fileName, vector<string>& mcode) {
	ifstream file(fileName);
	string codes, line;
	while (getline(file, line)) {
		mcode.push_back(line);
	}

	file.close();
}

vector<int> lps(string str) {
	int m = str.size();
	vector<int> lpsv(m, 0);

	for (int i = 1, j = 0; i < m;) {
		if (str[i] == str[j]) {
			lpsv[i++] = ++j;
		} else {
			if (!j) {
				lpsv[i++] = 0;
			} else {
				j = lpsv[j - 1];
			}
		}
	}

	return lpsv;
}

vector<int> kmp(string str, string pattern) {
	vector<int> lpsv = lps(pattern);
	vector<int> posMatch;
	int n = str.size(), m = pattern.size();
	int i = 0, j = 0;

	while (i < n) {
		if (str[i] == pattern[j]) {
			j++, i++;
			if (j == m) {
				posMatch.push_back(i - m);
				j = lpsv[j - 1];
			}
		} else {
			if (!j) {
				i++;
			} else {
				j = lpsv[j - 1];
			}
		}
	}

	return posMatch;
}

void searchCode(const string& transmision, const string& mcode, ofstream& outputFile, int numTrans) {
    vector<int> posMatch = kmp(transmision, mcode);
    
    outputFile << "transmission" << numTrans << ".txt ==> " << posMatch.size() << " veces\n";

    int i = 0;
    for(int pos : posMatch) {
    	outputFile << pos;
    	if (i++ < posMatch.size() - 1)
			outputFile << ", ";
    }

    outputFile << "\n";
}

string manacher(const string& str) {
    string T = "$";
    for (char s : str) {
        T += "|";
        T += s;
    }
    T += "|$";

    int n = T.size();
    vector<int> L(n);
    L[0] = 0;
    L[1] = 1;

    int maxLong = 0, maxMid = 0;
    bool exp;
    for (int c = 1, li = 0, ri = 2; ri < n; ri++) {
        li = c - (ri - c);
        exp = false;
        if (c - maxLong <= ri && ri >= c + maxLong) {
            if (L[li] < (c + L[c] - ri)) {
                L[ri] = L[li];
            } else if (L[li] == (c + L[c]) - ri && (c + L[c]) == 2 * n) {
                L[ri] = L[li];
            } else if (L[li] == (c + L[c]) - ri && ( c + L[c]) < 2 * n) {
                L[ri] = L[li];
                exp = true;
            } else if (L[li] > (c + L[c]) - ri && (c + L[c]) < 2 * n) {
                L[ri] = (c + L[c]) - ri;
                exp = true;
            }
        } else {
            L[ri] = 0;
            exp = true;
        }

        if (exp) {
            while ((ri + L[ri] < n) && (ri - L[ri] > 0) && 
                    (T[ri - L[ri] - 1] == T[ri + L[ri] + 1])) {
                L[ri]++;
            }
        }

        c = ri;
        if (L[ri] > maxLong) {
            maxLong = L[ri];
            maxMid = ri;
        }
    }

    int inicio = (maxMid - maxLong) / 2;
    string salida = str.substr(inicio, maxLong); // Usar substr para extraer el palíndromo más largo
    return salida;
}

void findLongestPalindrome(const string& transmision, ofstream& outputFile, int numTrans) {
    string longestPalindrome = manacher(transmision);
    // Aquí puedes encontrar la posición del palíndromo más largo en la cadena de transmisión original
    int position = transmision.find(longestPalindrome);
    outputFile << "Transmission" << numTrans << ".txt ==> Posición: " << position << "\n" << longestPalindrome << "\n----\n";
}

int main() {
	string trans1 = readFile("transmission1.txt");
	string trans2 = readFile("transmission2.txt");
	string trans3 = readFile("transmission3.txt");

	vector<string> mcode;
	readCodes("mcode.txt", mcode);

	ofstream outputFile("cheking.txt");

	int i = 0;
	for(const string& code : mcode) {
        outputFile << "Código: " << code << "\n";
        searchCode(trans1, code, outputFile, 1);
        searchCode(trans2, code, outputFile, 2);
        searchCode(trans3, code, outputFile, 3);    
        if (i++ < mcode.size() - 1)
        	outputFile << "--------------" << "\n";
    }

    outputFile << "==============" << "\n";
    outputFile << "Palíndromo más grande:" << "\n";
    findLongestPalindrome(trans1, outputFile, 1);
    findLongestPalindrome(trans2, outputFile, 2);
    findLongestPalindrome(trans3, outputFile, 3);

	outputFile.close();

	return 0;
}






