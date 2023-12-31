// Ana Carolina Ramírez González A00833324
// Roberto Andonie Hernández A01411863

// TC2038 - Actividad Integradora 1

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// O(n), donde n es el número de caracteres del archivo
string readFile(const string& fileName) {
	ifstream file(fileName);
	string transmission, line;
	while (getline(file, line)) {
		transmission += line;
	}

	file.close();
	return transmission;
}

// O(n), donde n es el número de códigos maliciosos
void readCodes(const string& fileName, vector<string>& mcode) {
	ifstream file(fileName);
	string codes, line;
	while (getline(file, line)) {
		mcode.push_back(line);
	}

	file.close();
}

// Complejidad: O(m) donde m es la longitud del patrón
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

// Complejidad: O(n + m) donde n es la longitud del string y m del patrón
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

// O(n * m) donde n y m son las longitudes de las dos cadenas
int subsequence(string s1, string s2) {
    int n = s1.length();
    int m = s2.length();
    vector<vector<int> > LCS(n, vector<int>(m, 0));

    if (s1[0] == s2[0]) {
        LCS[0][0] = 1;
    } else {
        LCS[0][0] = 0;
    }

    for (int i = 1; i < n; ++i) {
        if (s1[i] == s2[0]) {
            LCS[i][0] = 1;
        } else {
            LCS[i][0] = LCS[i - 1][0];
        }
    }

    for (int j = 1; j < m; ++j) {
        if (s1[0] == s2[j]) {
            LCS[0][j] = 1;
        } else {
            LCS[0][j] = LCS[0][j - 1];
        }
    }

    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            if (s1[i] == s2[j]) {
                LCS[i][j] = LCS[i - 1][j - 1] + 1;
            } else {
                LCS[i][j] = max(LCS[i - 1][j], LCS[i][j - 1]);
            }
        }
    }
    return LCS[n - 1][m - 1];
}


// Complejidad: O(n * m) donde n es la longitud del string y m de la subsequence
int countSubsequence(const string& str, const string& subsequence) {
    int count = 0;
    size_t pos = str.find(subsequence);
    while (pos != string::npos) {
        count++;
        pos = str.find(subsequence, pos + 1);
    }
    return count;
}

// Complejidad: O(n), donde n es la longitud del string
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
    string salida = str.substr(inicio, maxLong);
    return salida;
}

// O(n), donde n es la longitud de la transmisión
void findLongestPalindrome(const string& transmision, ofstream& outputFile, int numTrans) {
    string longestPalindrome = manacher(transmision);
    int position = transmision.find(longestPalindrome);
    outputFile << "Transmission" << numTrans << ".txt ==> Posición: " << position << "\n" << longestPalindrome << "\n----\n";
}

// O(n * m), donde n y m son las longitudes de las dos cadenas.
string findLongestSubstring(const string& s1, const string& s2) {
    int n = s1.length();
    int m = s2.length();
    vector<vector<int> > LCS(n + 1, vector<int>(m + 1, 0));

    int maxLength = 0;
    int endIndexS1 = 0;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                LCS[i][j] = LCS[i - 1][j - 1] + 1;
                if (LCS[i][j] > maxLength) {
                    maxLength = LCS[i][j];
                    endIndexS1 = i - 1;
                }
            } else {
                LCS[i][j] = 0;
            }
        }
    }

    if (maxLength == 0) {
        return "";
    }

    return s1.substr(endIndexS1 - maxLength + 1, maxLength);
}

// O(n * m * t), donde n es la longitud del código, 
// m es la longitud de la transmisión, y t es el número de transmisiones.
void searchAndRecordSubsequences(string code, ofstream& outputFile) {
    string mostCommonSubsequence = "";
    int mostCommonCount = 0;
    int transmission = 0;

    for(int transmissionNumber = 1; transmissionNumber <= 3; transmissionNumber++) {
        for(int i = 0; i < code.length(); i++) {
            string subseq = code.substr(0, i) + code.substr(i + 1);
            string trans = readFile("transmission" + to_string(transmissionNumber) + ".txt");
            int count = countSubsequence(trans, subseq);
            
            if(count > mostCommonCount) {
                mostCommonSubsequence = subseq;
                mostCommonCount = count;
                transmission = transmissionNumber;
            }
        }
    }

    outputFile << "La subsecuencia más encontrada es: " << mostCommonSubsequence
                       << " con " << mostCommonCount << " veces en el archivo Transmisión" << transmission << ".txt\n";
}


int main() {
    string trans1 = readFile("transmission1.txt");
    string trans2 = readFile("transmission2.txt");
    string trans3 = readFile("transmission3.txt");

    vector<string> mcode;
    readCodes("mcode.txt", mcode);

    ofstream outputFile("checking.txt");

    int i = 0;
    for (const string& code : mcode) {
        outputFile << "Código: " << code << "\n";

        vector<int> positions1 = kmp(trans1, code);
        vector<int> positions2 = kmp(trans2, code);
        vector<int> positions3 = kmp(trans3, code);

        outputFile << "Transmission1.txt ==> " << positions1.size() << " veces\n";
        if (!positions1.empty()) {
            outputFile << "Posiciones: ";
            for (int i = 0; i < positions1.size(); ++i) {
                outputFile << positions1[i];
                if (i < positions1.size() - 1) {
                    outputFile << ", ";
                }
            }
            outputFile << "\n";
        }

        outputFile << "Transmission2.txt ==> " << positions2.size() << " veces\n";
        if (!positions2.empty()) {
            outputFile << "Posiciones: ";
            for (int i = 0; i < positions2.size(); ++i) {
                outputFile << positions2[i];
                if (i < positions2.size() - 1) {
                    outputFile << ", ";
                }
            }
            outputFile << "\n";
        }

        outputFile << "Transmission3.txt ==> " << positions3.size() << " veces\n";
        if (!positions3.empty()) {
            outputFile << "Posiciones: ";
            for (int i = 0; i < positions3.size(); ++i) {
                outputFile << positions3[i];
                if (i < positions3.size() - 1) {
                    outputFile << ", ";
                }
            }
            outputFile << "\n";
        }
        searchAndRecordSubsequences(code, outputFile);
        
        outputFile << "--------------\n";
    }

    outputFile << "==============" << "\n";
    outputFile << "Palíndromo más grande:" << "\n";
    findLongestPalindrome(trans1, outputFile, 1);
    findLongestPalindrome(trans2, outputFile, 2);
    findLongestPalindrome(trans3, outputFile, 3);

    string longestSubstring1_2 = findLongestSubstring(trans1, trans2);
    string longestSubstring1_3 = findLongestSubstring(trans1, trans3);
    string longestSubstring2_3 = findLongestSubstring(trans2, trans3);

    outputFile << "==============" << "\n";
    outputFile << "Los Substring más largos son:" << "\n";
    outputFile << "T1-T2 ==> " << longestSubstring1_2 << "\n";
    outputFile << "T1-T3 ==> " << longestSubstring1_3 << "\n";
    outputFile << "T2-T3 ==> " << longestSubstring2_3 << "\n";

    outputFile.close();

    return 0;
}