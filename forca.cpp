#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

using namespace std;

class Palavra {
protected:

    string* palavraSecreta;

public:

    const string& getPalavraSecreta() const;
    Palavra();
    ~Palavra();
    void carregarPalavraSecreta(); // Busca palavra no arquivo e a insere no vetor palavraSecreta

};

class Forca : public Palavra {
private:

    vector<char> letrasErradas;
    vector<char> letrasCorretas;
    int erros;
    static int numTentativas;

public:

    static void mensagemBoasVindas(); // Mensagem abre jogo
    Forca();
    ~Forca();
    void exibirPalavra() const; // Para revelar a palavra na medida que as letras são acertadas
    void tentar(char letra); // Tentativa letra por letra (função sobrecarregada)
    void tentar(const string& tentativa); // Para arriscar um palpite na palavra inteira
    void exibirForca() const; // Montar graficamente a forca na medida que ocorrem os erros nas tentativas
    bool jogoTerminado() const; // Verificar se o jogo terminou e exibir o resultado
    bool palavraAdivinhada() const; // Checar se todas as letras foram descobertas
    const vector<char>& getLetrasErradas() const; // metodo para retornar letras erradas
    static int getNumTentativas(); // membro estático para contabilizar as tentativas
};

int Forca::numTentativas = 0;

ostream& operator<<(ostream& output, const Forca& jogo) { //Sobrecarga de Operador <<
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
        cout << "Digite uma letra ou arrisque o nome do time: ";
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
        cout << "Tentativas feitas: " << Forca::getNumTentativas() << endl;
    }
    else {
        jogo.exibirForca();
        cout << "Voce perdeu! O nome do time era: " << jogo.getPalavraSecreta() << endl;
        cout << "Tentativas feitas: " << Forca::getNumTentativas() << endl;
    }

    return 0;
}

// FUNÇÕES CLASSE PALAVRA

Palavra::Palavra() {
    carregarPalavraSecreta();
}

Palavra::~Palavra() {
    delete palavraSecreta;
    delete[] palavraSecreta;
}

void Palavra::carregarPalavraSecreta() {
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

const string& Palavra::getPalavraSecreta() const {
    return *palavraSecreta;
}


// FUNÇÕES CLASSE FORCA

void Forca::mensagemBoasVindas() {
    cout << "Bem-vindo ao jogo da Forca! Adivinhe o nome dos Times de Futebol" << endl;
}

Forca::Forca() : erros(0){}
Forca::~Forca(){}

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
        throw invalid_argument("Por favor, insira apenas letras.");
    }

    if (find(letrasCorretas.begin(), letrasCorretas.end(), tolower(letra)) != letrasCorretas.end() ||
        find(letrasErradas.begin(), letrasErradas.end(), tolower(letra)) != letrasErradas.end()) {
        throw logic_error("Voce ja tentou essa letra. Tente outra.");
    }

    size_t pos = palavraSecreta->find(letra);

    if (pos != string::npos) {
        letrasCorretas.push_back(letra);
        cout << "Letra correta!" << endl;
        numTentativas++;
    }
    else {
        letrasErradas.push_back(letra);
        cout << "Letra incorreta!" << endl;
        erros++;
        numTentativas++;
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
            numTentativas++;
        }
        else {
            numTentativas++;
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

const vector<char>& Forca::getLetrasErradas() const {
    return letrasErradas;
}

int Forca::getNumTentativas() {
    return numTentativas;
}
