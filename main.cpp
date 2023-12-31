#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <time.h>
#include <fstream>

#define AltezzaSchermo 700
#define LarghezzaSchermo 1370

#define Nlettere 26
#define MaxParole 1000
#define NumLingue 2

using namespace std;

sf::Font font;
char lettera='-';
string parola="IMPICCATO";
bool parolaScoperta[Nlettere], letteraScoperta[Nlettere];
int errori=0;
int fase=0;/// 0 menu, 1 partita normale, 2 impostazioni, 3 crediti, 4 seleziona modalita

string lingua[NumLingue]={"Italiano", "English"};
int indLingua=0;
int volume=50;

int tStartCredits;
int mouseWheel=0;

void aggiusta( string vocabolario[MaxParole])
{
    ofstream fout;
    fout.open("vocabolario.txt", ios::out);
    for(int i=0; i<MaxParole; i++)
    {
        for(int j=0; j>i; j++)
        {
            if(vocabolario[j]==vocabolario[i])  vocabolario[j]="-";
        }
        if(vocabolario[i].size()<4) vocabolario[i]="-";
    }
    for(int i=0; i<MaxParole; i++)
    {
        if(vocabolario[i]!="-") fout<<vocabolario[i]<<endl;
        //cout<<vocabolario[i]<<endl;
    }
}

string carica()
{
    string vocabolario[MaxParole];
    int nParole=0;
    ifstream fin("vocabolario.txt");
    srand(time(0));
    while(!fin.eof())
    {
        getline(fin, vocabolario[nParole]);
        nParole++;
    }
    nParole--;
    aggiusta(vocabolario);
    int n;
    do
    {
        n=rand()%nParole;
    }while(vocabolario[n]=="-");
    string parola=vocabolario[n];
    for(int i=0; i<parola.size(); i++)
    {
        if(parola[i]=='à') parola[i]='a';
        if(parola[i]=='ò') parola[i]='o';
        if(parola[i]=='è') parola[i]='e';
        if(parola[i]=='ì') parola[i]='i';
        if(parola[i]=='ù') parola[i]='u';
        if((int)parola[i]>96) parola[i]=parola[i]-32;
    }
    return parola;
}

void inizializza()
{
        parola=carica();

        for(int i=0; i<Nlettere; i++)
        {
            letteraScoperta[i]=0;
        }
        for(int i=0; i<parola.size(); i++)
        {
            parolaScoperta[i]=0;
            if(parola[i]==' ')  parolaScoperta[i]=1;
        }

        char lettera;

        lettera=parola[0];
        letteraScoperta[((int) lettera)-65]=1;
        for(int i=0; i<parola.size(); i++)
        {
            if(lettera==parola[i])
                parolaScoperta[i]=1;
        }
        lettera=parola[parola.size()-1];
        letteraScoperta[((int) lettera)-65]=1;
        for(int i=0; i<parola.size(); i++)
        {
            if(lettera==parola[i])
                parolaScoperta[i]=1;
        }
        errori=0;

        fase=1;
}

class pulsante
{
    public:
    int h;//altezza
    int l;//lunghezza
    int CX;//centro x
    int CY;//centro y


    sf::RectangleShape Forma;
    sf::Vector2i PositionMouse;
    sf::Text text;
    //sf::Font font;

    bool attivo()
    {
        return (PositionMouse.x>CX && PositionMouse.x<(CX+l) && PositionMouse.y>CY && PositionMouse.y<(CY+h));
    }

    void disegna(sf::RenderWindow * window)
    {
        window->draw(Forma);


        window->draw(text);
    }

    pulsante(string etichetta, int Altezza, int Larghezza, int centroX, int centroY, int textSize, sf::Color textColor, sf::Color buttonColor, sf::RenderWindow * window)
    {
        h=Altezza;
        l=Larghezza;
        CX=centroX;
        CY=centroY;


        text.setFont(font);
        text.setString(etichetta);
        text.setFillColor(textColor);
        text.setStyle(sf::Text::Bold);
        text.setCharacterSize(textSize);
        text.setPosition((centroX+(l-etichetta.length()*((textSize)/2))/2)*1.f, (centroY+(h-textSize)/2)*1.f);

        Forma.setFillColor(buttonColor);
        Forma.setSize(sf::Vector2f(Larghezza*1.f, Altezza*1.f));
        Forma.setPosition(centroX*1.f, centroY*1.f);

        PositionMouse = sf::Mouse::getPosition(*window);
    }
};

SelModalita(sf::RenderWindow *window)
{
    {
        static bool premuto;
        pulsante Menu("Menu", 50, 70, 1260, 40, 20, sf::Color::White, sf::Color::Black, window);
        if(Menu.attivo())
        {
            Menu.Forma.setFillColor(sf::Color(126, 126, 126, 126));
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                premuto=1;
            }
            else
            {
                if(premuto) fase=0;
                premuto=0;
            }
        }
        else
            premuto=0;

        Menu.disegna(window);
    }

    ///Barra volume
    {
        int Altezza=40;
        int Larghezza=350;
        int centroX=510;
        int centroY=230+110;

                sf::RectangleShape BackGroundBar;
                sf::RectangleShape Barra;
                sf::Vector2i PositionMouse = sf::Mouse::getPosition(*window);
                sf::Text textVolume;
                sf::Text textPercentuale;

                textVolume.setString("Volume");
                textVolume.setFont(font);
                textVolume.setCharacterSize(30);
                textVolume.setFillColor(sf::Color::Black);
                textVolume.setStyle(sf::Text::Bold);
                textVolume.setPosition((centroX+15)*1.f, (centroY-40)*1.f);

                string perc;
                perc=char((volume-volume%10)/10+48);
                perc=perc+ char(volume%10+48);
                perc.append("%");

                textPercentuale.setString(perc);
                textPercentuale.setFont(font);
                textPercentuale.setCharacterSize(20);
                textPercentuale.setFillColor(sf::Color::White);
                textPercentuale.setStyle(sf::Text::Bold);
                textPercentuale.setPosition((centroX+Larghezza/2-25)*1.f, (centroY+5)*1.f);

                BackGroundBar.setFillColor(sf::Color(126, 126, 126, 126));
                BackGroundBar.setSize(sf::Vector2f((Larghezza)*1.f, (Altezza)*1.f));
                BackGroundBar.setPosition((centroX)*1.f, (centroY)*1.f);

                Barra.setFillColor(sf::Color::Black);
                Barra.setSize(sf::Vector2f((Larghezza*volume)/100*1.f, Altezza*1.f));
                Barra.setPosition((centroX)*1.f, (centroY)*1.f);

                if(PositionMouse.x>(centroX) && PositionMouse.x<(centroX+Larghezza) && PositionMouse.y>(centroY) && PositionMouse.y<(centroY+Altezza))
                    volume=(PositionMouse.x-centroX)*100/Larghezza;

                window->draw(BackGroundBar);
                window->draw(Barra);
                window->draw(textVolume);
                window->draw(textPercentuale);
    }

    {//lingua
        static bool premuto;
        pulsante PulsanteLingua("Lingua:  "+lingua[indLingua%NumLingue], 100, 350, 510, 200+2*110, 40, sf::Color::White, sf::Color::Black, window);
        if(PulsanteLingua.attivo())
        {
            PulsanteLingua.Forma.setFillColor(sf::Color(126, 126, 126, 126));
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                premuto=1;
            }
            else
            {
                if(premuto) indLingua++;
                premuto=0;
            }
        }
        else
            premuto=0;

        PulsanteLingua.disegna(window);
        }


    pulsante Esci("Esci", 100, 350, 510, 200+3*110, 50, sf::Color::White, sf::Color::Black, window);
    if(Esci.attivo())
    {
        Esci.Forma.setFillColor(sf::Color(126, 126, 126, 126));
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
                window->close();
        }
    }
    Esci.disegna(window);
}

void Gioco(sf::RenderWindow *window)
{
    {//grafica

        {//forca
        sf::RectangleShape Forca;
        Forca.setFillColor(sf::Color::Black);

        Forca.setSize(sf::Vector2f(20.f, 500.f));
        Forca.setPosition(100.f, 70.f);
        window->draw(Forca);

        Forca.setSize(sf::Vector2f(400.f, 20.f));
        Forca.setPosition(50.f, 50.f);
        window->draw(Forca);

        Forca.setSize(sf::Vector2f(170.f, 20.f));
        Forca.setPosition(30.f, 570.f);
        window->draw(Forca);

        Forca.setSize(sf::Vector2f(20.f, 165.f));
        Forca.setPosition(114.f, 187.f);
        Forca.rotate(360-135);
        window->draw(Forca);
        }

        {//corpo
        if(errori>=3)
        {   //testa
            sf::CircleShape testa(50.f, 4);
            testa.setFillColor(sf::Color::Black);
            testa.setPosition(300.f, 80.f);
            window->draw(testa);
        }

        if(errori>=6)
        {    //torso
            sf::RectangleShape Torso;
            Torso.setFillColor(sf::Color::Black);

            Torso.setSize(sf::Vector2f(100.f, 70.f));
            Torso.setPosition(300.f, 190.f);
            window->draw(Torso);

            Torso.setSize(sf::Vector2f(20.f, 100.f));
            Torso.setPosition(340.f, 260.f);
            window->draw(Torso);
        }

        if(errori>=9)
        {   //braccio sinistro
            sf::RectangleShape Torso;
            Torso.setFillColor(sf::Color::Black);

            Torso.setSize(sf::Vector2f(20.f, 200.f));
            Torso.setPosition(270.f, 195.f);
            window->draw(Torso);
        }

        if(errori>=12)
        {   //braccio destro
            sf::RectangleShape Torso;
            Torso.setFillColor(sf::Color::Black);

            Torso.setSize(sf::Vector2f(20.f, 200.f));
            Torso.setPosition(410.f, 195.f);
            window->draw(Torso);
        }

        if(errori>=15)
        {   //gamba sinistra
            sf::RectangleShape Torso;
            Torso.setFillColor(sf::Color::Black);

            Torso.setSize(sf::Vector2f(20.f, 200.f));
            Torso.setPosition(310.f, 370.f);
            window->draw(Torso);
        }

        if(errori>=18)
        {   //gamba destra
            sf::RectangleShape Torso;
            Torso.setFillColor(sf::Color::Black);

            Torso.setSize(sf::Vector2f(20.f, 200.f));
            Torso.setPosition(370.f, 370.f);
            window->draw(Torso);
        }
    }

        {//pulsanti lettere
        lettera='-';
        for(int i=0; i<26; i++)
        {
            string simbolo;
            simbolo=(char) (i+65);

            pulsante comodo(simbolo, 60, 60, 720+(i%7)*80, 50+(i/7)*80, 55, sf::Color::Black, sf::Color(126, 126, 126), window);

            if(!letteraScoperta[i])
            {
                comodo.Forma.setFillColor(sf::Color::White);
                if(comodo.attivo())
                {
                    comodo.Forma.setFillColor(sf::Color(126, 126, 126, 126));
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        lettera=simbolo[0];

                    }
                }
            }
            comodo.disegna(window);
        }

        {//parola indovinata
            sf::Text text;
            string parolaStampa="";
            for(int i=0; i<parola.size(); i++)
            {

                if(!parolaScoperta[i]) parolaStampa=parolaStampa+" _";
                else                   parolaStampa=parolaStampa+parola[i]+' ';
            }
            text.setFont(font);
            text.setString(parolaStampa);
            text.setFillColor(sf::Color::Black);
            text.setStyle(sf::Text::Bold);
            text.setCharacterSize(50);
            text.setPosition(700.f, 500.f);
            window->draw(text);
        }
    }

        }

        {//controllo
        if(lettera!='-')
        {
            bool sbagliato=1;
            letteraScoperta[((int) lettera)-65]=1;
            for(int i=0; i<parola.size(); i++)
            {
                if(lettera==parola[i])
                {
                    sbagliato=0;
                    parolaScoperta[i]=1;
                }
            }
            if(sbagliato)   errori+=3;
            lettera='-';
        }
    }

}

void Sconfitta(sf::RenderWindow *window)
{
    sf::Text text;
        text.setFont(font);
        text.setString("Accidenti!\nLa parola era \n\t"+parola+"\nVuoi giocare ancora?");
        text.setFillColor(sf::Color::Black);
        text.setStyle(sf::Text::Bold);
        text.setCharacterSize(50);
        text.setPosition(350.f, 150.f);
        window->draw(text);

        pulsante GiocaAncora("Gioca ancora", 100, 350, 300, 400, 50, sf::Color::White, sf::Color::Black, window);
        if(GiocaAncora.attivo())
        {
            GiocaAncora.Forma.setFillColor(sf::Color(126, 126, 126, 126));
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                inizializza();
            }
        }
        GiocaAncora.disegna(window);

        {
        static bool premuto;
        pulsante Menu("Menu", 100, 350, 700, 400, 50, sf::Color::White, sf::Color::Black, window);
        if(Menu.attivo())
        {
            Menu.Forma.setFillColor(sf::Color(126, 126, 126, 126));
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                premuto=1;
            }
            else
            {
                if(premuto) fase=0;
                premuto=0;
            }
        }
        else
            premuto=0;

        Menu.disegna(window);
        }
}

void Vittoria(sf::RenderWindow *window)
{
    sf::Text text;
        text.setFont(font);
        text.setString("Complimenti!\nHai indovinato la parola \n\t"+parola+"\nVuoi giocare ancora?");
        text.setFillColor(sf::Color::Black);
        text.setStyle(sf::Text::Bold);
        text.setCharacterSize(50);
        text.setPosition(350.f, 150.f);
        window->draw(text);

        pulsante GiocaAncora("Gioca ancora", 100, 350, 300, 400, 50, sf::Color::White, sf::Color::Black, window);
        if(GiocaAncora.attivo())
        {
            GiocaAncora.Forma.setFillColor(sf::Color(126, 126, 126, 126));
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                inizializza();
            }
        }
        GiocaAncora.disegna(window);
        {
        static bool premuto;
        pulsante Menu("Menu", 100, 350, 700, 400, 50, sf::Color::White, sf::Color::Black, window);
        if(Menu.attivo())
        {
            Menu.Forma.setFillColor(sf::Color(126, 126, 126, 126));
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                premuto=1;
            }
            else
            {
                if(premuto) fase=0;
                premuto=0;
            }
        }
        else
            premuto=0;

        Menu.disegna(window);
        }

}

void Impostazioni(sf::RenderWindow *window)
{
    {
        static bool premuto;
        pulsante Menu("Menu", 50, 70, 1260, 40, 20, sf::Color::White, sf::Color::Black, window);
        if(Menu.attivo())
        {
            Menu.Forma.setFillColor(sf::Color(126, 126, 126, 126));
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                premuto=1;
            }
            else
            {
                if(premuto) fase=0;
                premuto=0;
            }
        }
        else
            premuto=0;

        Menu.disegna(window);
    }

    ///Barra volume
    {
        int Altezza=40;
        int Larghezza=350;
        int centroX=510;
        int centroY=230+110;

                sf::RectangleShape BackGroundBar;
                sf::RectangleShape Barra;
                sf::Vector2i PositionMouse = sf::Mouse::getPosition(*window);
                sf::Text textVolume;
                sf::Text textPercentuale;

                textVolume.setString("Volume");
                textVolume.setFont(font);
                textVolume.setCharacterSize(30);
                textVolume.setFillColor(sf::Color::Black);
                textVolume.setStyle(sf::Text::Bold);
                textVolume.setPosition((centroX+15)*1.f, (centroY-40)*1.f);

                string perc;
                perc=char((volume-volume%10)/10+48);
                perc=perc+ char(volume%10+48);
                perc.append("%");

                textPercentuale.setString(perc);
                textPercentuale.setFont(font);
                textPercentuale.setCharacterSize(20);
                textPercentuale.setFillColor(sf::Color::White);
                textPercentuale.setStyle(sf::Text::Bold);
                textPercentuale.setPosition((centroX+Larghezza/2-25)*1.f, (centroY+5)*1.f);

                BackGroundBar.setFillColor(sf::Color(126, 126, 126, 126));
                BackGroundBar.setSize(sf::Vector2f((Larghezza)*1.f, (Altezza)*1.f));
                BackGroundBar.setPosition((centroX)*1.f, (centroY)*1.f);

                Barra.setFillColor(sf::Color::Black);
                Barra.setSize(sf::Vector2f((Larghezza*volume)/100*1.f, Altezza*1.f));
                Barra.setPosition((centroX)*1.f, (centroY)*1.f);

                if(PositionMouse.x>(centroX) && PositionMouse.x<(centroX+Larghezza) && PositionMouse.y>(centroY) && PositionMouse.y<(centroY+Altezza))
                    volume=(PositionMouse.x-centroX)*100/Larghezza;

                window->draw(BackGroundBar);
                window->draw(Barra);
                window->draw(textVolume);
                window->draw(textPercentuale);
    }

    {//lingua
        static bool premuto;
        pulsante PulsanteLingua("Lingua:  "+lingua[indLingua%NumLingue], 100, 350, 510, 200+2*110, 40, sf::Color::White, sf::Color::Black, window);
        if(PulsanteLingua.attivo())
        {
            PulsanteLingua.Forma.setFillColor(sf::Color(126, 126, 126, 126));
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                premuto=1;
            }
            else
            {
                if(premuto) indLingua++;
                premuto=0;
            }
        }
        else
            premuto=0;

        PulsanteLingua.disegna(window);
        }


    pulsante Esci("Esci", 100, 350, 510, 200+3*110, 50, sf::Color::White, sf::Color::Black, window);
    if(Esci.attivo())
    {
        Esci.Forma.setFillColor(sf::Color(126, 126, 126, 126));
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
                window->close();
        }
    }
    Esci.disegna(window);
}

void Crediti(sf::RenderWindow *window)
{
    {///Menu
        static bool premuto;
        pulsante Menu("Menu", 50, 70, 1260, 40, 20, sf::Color::White, sf::Color::Black, window);
        if(Menu.attivo())
        {
            Menu.Forma.setFillColor(sf::Color(126, 126, 126, 126));
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                premuto=1;
            }
            else
            {
                if(premuto) fase=0;
                premuto=0;
            }
        }
        else
            premuto=0;

        Menu.disegna(window);
        }


    {//testo
        sf::Event event;
        ifstream fin("Crediti.txt");
        string testo[100];
        sf::Text TitoliDiCoda;

        TitoliDiCoda.setCharacterSize(40.f);
        TitoliDiCoda.setFont(font);
        TitoliDiCoda.setColor(sf::Color::Black);

        int n=0;

        while(n<100 && !fin.eof())
        {
            getline(fin, testo[n]);
            n++;
        }

        for(int i=0; i<n; i++)
        {
            TitoliDiCoda.setString(testo[i]);
            //sf::Event::MouseWheelMoved;
            TitoliDiCoda.setPosition(550.f, (700+i*50-(time(0)-tStartCredits)*30/*+ event.mouseWheel.y*/)*1.f);
            window->draw(TitoliDiCoda);

        }


    }
}

void SchermataInizio(sf::RenderWindow *window)
{
    sf::Text text;
    text.setFont(font);
    text.setString("L'IMPICCATO");
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);
    text.setCharacterSize(60);
    text.setPosition(500.f, 10.f);
    window->draw(text);


    pulsante GiocaAncora("Nuova partita", 100, 350, 510, 200, 50, sf::Color::White, sf::Color::Black, window);
    if(GiocaAncora.attivo())
    {
        GiocaAncora.Forma.setFillColor(sf::Color(126, 126, 126, 126));
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
                inizializza();
        }
    }
    GiocaAncora.disegna(window);

    pulsante pImpostazioni("Impostazioni", 100, 350, 510, 200+110, 50, sf::Color::White, sf::Color::Black, window);
    if(pImpostazioni.attivo())
    {
        pImpostazioni.Forma.setFillColor(sf::Color(126, 126, 126, 126));
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
                fase=2;
        }
    }
    pImpostazioni.disegna(window);

    pulsante pCrediti("Crediti", 100, 350, 510, 200+110*2, 50, sf::Color::White, sf::Color::Black, window);
    if(pCrediti.attivo())
    {
        pCrediti.Forma.setFillColor(sf::Color(126, 126, 126, 126));
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
                fase=3;
                tStartCredits=time(0);
        }
    }
    pCrediti.disegna(window);


}

void stampa(sf::RenderWindow *window)
{
    bool acc=1;
    for(int i=0; i<parola.size(); i++)
    acc=acc&&parolaScoperta[i];

    switch(fase)
    {
        case 0:
            SchermataInizio(window);
        break;

        case 1:
            if(acc) Vittoria(window);
            else
            {
                if(errori>=18)  Sconfitta(window);
                else    Gioco(window);
            }
        break;

        case 2:
            Impostazioni(window);
        break;

        case 3:
            Crediti(window);
        break;

        case 4:
            SelModalita(window);
        break;
    }
}

void nextFrame(sf::RenderWindow *window)
{
    sf::Event event;
    while (window->isOpen())
    {
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }
        window->clear(sf::Color::White);
        stampa(window);
        window->display();

        if (event.type == sf::Event::MouseWheelMoved)
        {
            std::cout << "wheel movement: " << event.mouseWheel.delta << std::endl;
            std::cout << "mouse x: " << event.mouseWheel.x << std::endl;
            std::cout << "mouse y: " << event.mouseWheel.y << std::endl;
        }


    }

}

int main()
{
    sf::String playerInput;
    sf::Text playerText;
    sf::RenderWindow window(sf::VideoMode(LarghezzaSchermo, AltezzaSchermo), "Display");
    window.setPosition(sf::Vector2i(-10, 0));
    sf::RenderWindow * windowPointer=&window;

    if (!font.loadFromFile("arial.ttf"))
    {
        cout<<"Errore";// error...
    }


    nextFrame(windowPointer);


    return 0;
}
