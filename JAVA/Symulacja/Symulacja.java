package symulacja;

//Program symuluje epidemie.

public class Symulacja {

    public static void main(String[] args) {
        Wczytywania w=new Wczytywania();
        w.ustawAgentów();
        Epidemia e=w.przygotujSymulację();
        e.przeprowadźSymulację();
    }
    
}
