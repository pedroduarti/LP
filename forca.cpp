#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

using namespace std;

class Forca {
private:
    string* palavraSecreta;
    vector<char> letrasErradas;
    vector<char> letrasCorretas;
    int erros;

public:
    static void mensagemBoasVindas();

    Forca();

    ~Forca();

    void carregarPalavraSecreta();

    void exibirPalavra()const;

    void tentar(char letra);

    void tentar(const string& tentativa);

    void exibirForca() const;

    bool jogoTerminado() const;

    bool palavraAdivinhada() const;

    const string& getPalavraSecreta() const;

    const vector<char>& getLetrasErradas() const;
};

ostream& operator<<(ostream& output, const Forca& jogo) {
    output << "Palavra: ";
    jogo.exibirPalavra();
    output << "Letras erradas: ";

    for (char letra : jogo.getLetrasErradas()) {
            output << letra << " ";
    }
    output << endl;
    jogo.exibirForca();

    return output;
}

int main() {

        Forca::mensagemBoasVindas();
        Forca jogo;

        while (!jogo.jogoTerminado()) {
            cout << jogo;

            string tentativa;
            cout << "Digite uma letra ou o nome do time: ";
            getline(cin, tentativa);

            try {
                jogo.tentar(tentativa);
            } catch (const logic_error& e) {
                cerr << "Erro: " << e.what() << endl;
                cout << "Tente novamente." << endl;
            }

            cout << endl;
        }

        if (jogo.palavraAdivinhada()) {
            cout << "Parabens! Voce adivinhou o time: " << jogo.getPalavraSecreta() << endl;
        }
        else {
            jogo.exibirForca();
            cout << "Voce perdeu! O nome do time era: " << jogo.getPalavraSecreta() << endl;
        }

    return 0;
}

    void Forca::mensagemBoasVindas() {
        cout << "Bem-vindo ao jogo da Forca! Adivinhe o nome dos Times de Futebol" << endl;
    }

    Forca::Forca() : erros(0) {
        carregarPalavraSecreta();
    }

    Forca::~Forca() {
        delete palavraSecreta;
        delete[] palavraSecreta;
    }



    void Forca::carregarPalavraSecreta() {
        ifstream arquivo("palavras.txt");

        if (!arquivo.is_open()) {
            throw runtime_error("Erro ao abrir palavras.txt");
        }

        vector<string> palavrasDisponiveis;
        string linha;

        while (getline(arquivo, linha)) {
            transform(linha.begin(), linha.end(), linha.begin(), ::tolower);
            palavrasDisponiveis.push_back(linha);
        }

        arquivo.close();

        srand(time(0));
        int indice = rand() % palavrasDisponiveis.size();

        palavraSecreta = new string(palavrasDisponiveis[indice]);
    }

    void Forca::exibirPalavra() const {
        for (char letra : *palavraSecreta) {
            if (letra == ' ') {
                cout << "  ";
            }
            else if (find(letrasCorretas.begin(), letrasCorretas.end(), tolower(letra)) != letrasCorretas.end()) {
                cout << letra << " ";
            }
            else {
                cout << "_ ";
            }
        }
        cout << endl;
    }

    void Forca::tentar(char letra) {
        if (!isalpha(letra)) {
            throw invalid_argument("Por favor, insira uma letra valida.");
        }

        if (find(letrasCorretas.begin(), letrasCorretas.end(), tolower(letra)) != letrasCorretas.end() ||
            find(letrasErradas.begin(), letrasErradas.end(), tolower(letra)) != letrasErradas.end()) {
            throw logic_error("Voce ja tentou essa letra. Tente outra.");
        }

        size_t pos = palavraSecreta->find(letra);

        if (pos != string::npos) {
            letrasCorretas.push_back(letra);
            cout << "Letra correta!" << endl;
        }
        else {
            letrasErradas.push_back(letra);
            cout << "Letra incorreta. Tente novamente." << endl;
            erros++;
        }
    }

    void Forca::tentar(const string& tentativa) {
        if (tentativa.length() == 1) {
            tentar(tentativa[0]);
        }
        else if (tentativa.length() > 1) {

            string tentativaMinuscula = tentativa;
            transform(tentativaMinuscula.begin(), tentativaMinuscula.end(), tentativaMinuscula.begin(), ::tolower);

            if (tentativaMinuscula == *palavraSecreta) {
                letrasCorretas.assign(palavraSecreta->begin(), palavraSecreta->end());
            }
            else {
                erros = 6;
            }
        }
    }

    void Forca::exibirForca() const {

        switch (erros) {
            case 1:
                cout << "+-----+\n" << "|     |\n" << "|\n" << "|\n" << "|    /\n" << "|\n" << "=========" << endl;
                break;
            case 2:
                cout << "+-----+\n" << "|     |\n" <<  "|\n" << "|\n" << "|    / \\\n" << "|\n" << "=========\n" << endl;
                break;
            case 3:
                cout << "+-----+\n" << "|     |\n" <<  "|\n" << "|     |\n" << "|    / \\\n" << "|\n" << "=========\n" << endl;
                break;
            case 4:
                cout << "+-----+\n" << "|     |\n" <<  "|\n" << "|     |\\\n" << "|    / \\\n" << "|\n" << "=========\n" << endl;
                break;
            case 5:
                cout << "+-----+\n" << "|     |\n" <<  "|\n" << "|    /|\\\n" << "|    / \\\n" << "|\n" << "=========\n" << endl;
                break;
            case 6:
                cout << "+-----+\n" << "|     |\n" <<  "|     0\n" << "|    /|\\\n" << "|    / \\\n" << "|\n" << "=========\n" << endl;
        }
    }

    bool Forca::jogoTerminado() const {
        return palavraAdivinhada() || erros >= 6;
    }

    bool Forca::palavraAdivinhada() const {
    for (char letra : *palavraSecreta) {
        if (letra != ' ' && find(letrasCorretas.begin(), letrasCorretas.end(), tolower(letra)) == letrasCorretas.end()) {
            return false;
        }
    }
    return true;
    }

    const string& Forca::getPalavraSecreta() const {
        return *palavraSecreta;
    }

    const vector<char>& Forca::getLetrasErradas() const {
        return letrasErradas;
    }
