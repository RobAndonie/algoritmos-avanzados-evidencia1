// Ana Carolina Ramírez González A00833324
// Roberto Andonie Hernández A01411863

// TC2038 - Actividad Integradora 1

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Complejidad: O(n)
string readFile(const string& fileName) {
	ifstream file(fileName);
	string transmission, line;
	while (getline(file, line)) {
		transmission += line;
	}

	file.close();
	return transmission;
}
// Complejidad: O(m)
void readCodes(const string& fileName, vector<string>& mcode) {
	ifstream file(fileName);
	string codes, line;
	while (getline(file, line)) {
		mcode.push_back(line);
	}

	file.close();
}

// Complejidad: O(m)
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
// Complejidad: O(n + m)
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
// Complejidad: O(n * m)
int subsequence(string s1, string s2) {
    int n = s1.length();
    int m = s2.length();
    vector<vector<int> > LCS(n + 1, vector<int>(m + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                LCS[i][j] = LCS[i - 1][j - 1] + 1;
            } else {
                LCS[i][j] = max(LCS[i - 1][j], LCS[i][j - 1]);
            }
        }
    }

    return LCS[n][m];
}

vector<int> findSubsequencePositions(const string& str, const string& subsequence) {
    vector<int> positions;
    size_t pos = str.find(subsequence);
    while (pos != string::npos) {
        positions.push_back(pos);
        pos = str.find(subsequence, pos + 1);
    }
    return positions;
}

// Complejidad: O(n * m)
int countSubsequence(const string& str, const string& subsequence) {
    int count = 0;
    size_t pos = str.find(subsequence);
    while (pos != string::npos) {
        count++;
        pos = str.find(subsequence, pos + 1);
    }
    return count;
}

// Complejidad: O(n * m)
void searchCode(const string& transmission, const string& code, ofstream& outputFile, int transmissionNumber) {
    vector<int> positions = kmp(transmission, code);
    int count = positions.size();

    outputFile << "Código: " << code << "\n";
    
    outputFile << "Transmission" << transmissionNumber << ".txt ==> " << count << " veces\n";
    
    if (count > 0) {
        outputFile << "Posiciones: ";
        for (int i = 0; i < count; ++i) {
            outputFile << positions[i];
            if (i < count - 1) {
                outputFile << ", ";
            }
        }
        outputFile << "\n";
        
        string mostCommonSubsequence = "";
        int mostCommonCount = 0;

        int maxSubsequenceCount = 0;

        for (int i = 0; i < count; ++i) {
            for (int j = i + 1; j < count; ++j) {
                string subsequence = transmission.substr(positions[i], positions[j] - positions[i] + code.length());
                int subsequenceCount = countSubsequence(transmission, subsequence);
                if (subsequenceCount > maxSubsequenceCount) {
                    mostCommonSubsequence = subsequence;
                    maxSubsequenceCount = subsequenceCount;
                }
            }
        }

        if (!mostCommonSubsequence.empty()) {
            outputFile << "La subsecuencia más encontrada es: " << mostCommonSubsequence
                       << " con " << maxSubsequenceCount << " veces en el archivo Transmission" << transmissionNumber << ".txt.\n";
        }
    }

    outputFile << "--------------\n";
}


// Complejidad: O(n)
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

// Complejidad: O(n^2)
void findLongestPalindrome(const string& transmision, ofstream& outputFile, int numTrans) {
    string longestPalindrome = manacher(transmision);
    int position = transmision.find(longestPalindrome);
    outputFile << "Transmission" << numTrans << ".txt ==> Posición: " << position << "\n" << longestPalindrome << "\n----\n";
}

// Complejidad: O(n*m)
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

int main() {
    string trans1 = readFile("Transmission1.txt");
    string trans2 = readFile("Transmission2.txt");
    string trans3 = readFile("Transmission3.txt");

    vector<string> mcode;
    readCodes("mcode.txt", mcode);

    ofstream outputFile("checking.txt");

    for (const string& code : mcode) {
        vector<int> positions1 = kmp(trans1, code);
        vector<int> positions2 = kmp(trans2, code);
        vector<int> positions3 = kmp(trans3, code);

        int lcs1 = subsequence(trans1, code);  
        int lcs2 = subsequence(trans2, code);  
        int lcs3 = subsequence(trans3, code); 

        int maxLCS = max(lcs1, max(lcs2, lcs3));

        outputFile << "Código: " << code << "\n";

        if (lcs1 > 0) {
            outputFile << "Transmission1.txt ==> " << lcs1 << " veces\n";
            outputFile << "Posiciones: ";
            for (int i = 0; i < lcs1; ++i) {
                outputFile << positions1[i];
                if (i < lcs1 - 1) {
                    outputFile << ", ";
                }
            }
            outputFile << "\n";
        } else {
            outputFile << "Transmission1.txt ==> 0 veces\n";
        }

        if (lcs2 > 0) {
            outputFile << "Transmission2.txt ==> " << lcs2 << " veces\n";
            outputFile << "Posiciones: ";
            for (int i = 0; i < lcs2; ++i) {
                outputFile << positions2[i];
                if (i < lcs2 - 1) {
                    outputFile << ", ";
                }
            }
            outputFile << "\n";
        } else {
            outputFile << "Transmission2.txt ==> 0 veces\n";
        }

        if (lcs3 > 0) {
            outputFile << "Transmission3.txt ==> " << lcs3 << " veces\n";
            outputFile << "Posiciones: ";
            for (int i = 0; i < lcs3; ++i) {
                outputFile << positions3[i];
                if (i < lcs3 - 1) {
                    outputFile << ", ";
                }
            }
            outputFile << "\n";
        } else {
            outputFile << "Transmission3.txt ==> 0 veces\n";
        }

        if (maxLCS > 0) {
            if (lcs1 == maxLCS) {
                outputFile << "La subsecuencia más encontrada es: " << code << " con " << lcs1 << " veces en el archivo Transmission1.txt.\n";
            }
            if (lcs2 == maxLCS) {
                outputFile << "La subsecuencia más encontrada es: " << code << " con " << lcs2 << " veces en el archivo Transmission2.txt.\n";
            }
            if (lcs3 == maxLCS) {
                outputFile << "La subsecuencia más encontrada es: " << code << " con " << lcs3 << " veces en el archivo Transmission3.txt.\n";
            }
        }

        outputFile << "--------------\n";
    }

    outputFile << "==============\n";

    // Palíndromos
    outputFile << "Palíndromo más grande:\n";
    findLongestPalindrome(trans1, outputFile, 1);
    findLongestPalindrome(trans2, outputFile, 2);
    findLongestPalindrome(trans3, outputFile, 3);

    // Substrings
    string longestSubstring1_2 = findLongestSubstring(trans1, trans2);
    string longestSubstring1_3 = findLongestSubstring(trans1, trans3);
    string longestSubstring2_3 = findLongestSubstring(trans2, trans3);

    outputFile << "==============\n";
    outputFile << "Los Substring más largos son:\n";
    outputFile << "T1-T2 ==> " << longestSubstring1_2 << "\n";
    outputFile << "T1-T3 ==> " << longestSubstring1_3 << "\n";
    outputFile << "T2-T3 ==> " << longestSubstring2_3 << "\n";

    outputFile.close();

    return 0;
}