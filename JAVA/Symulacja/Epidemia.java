package symulacja;

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Random;

public class Epidemia {
    private int seed;
    private int liczbaAgentów;
    private int liczbaDni;
    private double prawdTowarzyski;
    private int średniaLiczbaZnajomych;
    private Agent[] agenci;
    private PrintWriter writer;
    
    public Epidemia(int s, int la, int ld, double pt, PrintWriter w, int ślz){
        seed=s;
        liczbaAgentów=la;
        liczbaDni=ld;
        prawdTowarzyski=pt;
        agenci=new Agent[liczbaAgentów];
        writer=w;
        średniaLiczbaZnajomych=ślz;
        losujAgentów();
    }
    
    private void losujAgentów(){
        Random r=new Random(seed);
        for(int i=0;i<liczbaAgentów;++i){
            if(r.nextDouble()<=prawdTowarzyski) agenci[i]=new Towarzyski(i+1, liczbaDni);
            else agenci[i]=new Zwykly(i+1, liczbaDni);
        }
        final int stała=liczbaAgentów*średniaLiczbaZnajomych/2;
        int pom1, pom2;
        for(int i=0;i<stała;++i){
            pom1=r.nextInt(liczbaAgentów);
            pom2=r.nextInt(liczbaAgentów);
            while(agenci[pom1].czyMaWZnajomych(agenci[pom2]) || pom1==pom2){
                pom1=r.nextInt(liczbaAgentów);
                pom2=r.nextInt(liczbaAgentów);
            }
            agenci[pom1].dodajZnajomego(agenci[pom2]);
            agenci[pom2].dodajZnajomego(agenci[pom1]);
        }
        agenci[r.nextInt(liczbaAgentów)].zachoruj();
    }
    
    public void przeprowadźSymulację(){
        drukujPoczątkoweInfo();
        for(int i=0;i<liczbaDni;++i){
            for(int j=0;j<liczbaAgentów;++j){
                if(agenci[j].getŻywy()){
                    agenci[j].losujSpotknie(i);
                    agenci[j].spotkajSię(i);
                    agenci[j].nastDzień();
                }
            }
            podsumujDzień();
        }
    }
    
    private void podsumujDzień(){
        int z=0, o=0, c=0;
        for(int i=0;i<liczbaAgentów;++i){
            if(agenci[i].getŻywy()){
                if(agenci[i].getOdporny()) ++o;
                else{
                    if(agenci[i].getZdrowy()) ++z;
                    else ++c;
                }
            }    
        }
        writer.println(z+" "+c+" "+o);
        writer.flush();
    }
    
    private void drukujPoczątkoweInfo(){
        writer.println("# twoje wyniki powinny zawierać te komentarze");
        writer.format("seed=%d%n", seed);
        writer.format("liczbaAgentów=%d%n", liczbaAgentów);
        writer.format("prawdTowarzyski=%f%n", prawdTowarzyski);
        writer.format("prawdSpotkania=%f%n", Agent.prawdSpotkania);
        writer.format("prawdZarażenia=%f%n", Agent.prawdZarażenia);
        writer.format("prawdWyzdrowienia=%f%n", Agent.prawdWyzdrowienia);
        writer.format("śmiertelność=%f%n", Agent.śmiertelność);
        writer.format("liczbaDni=%d%n", liczbaDni);
        writer.format("śrZnajomych=%d%n", średniaLiczbaZnajomych);
        writer.println();
        writer.println("# agenci jako: id typ lub id* typ dla chorego");
        for(int i=0;i<liczbaAgentów;++i){
            if(agenci[i].getZdrowy()) writer.println(agenci[i].getPesel()+" "+agenci[i].getClass().getSimpleName());
            else writer.println(agenci[i].getPesel()+"* "+agenci[i].getClass().getSimpleName());
        }
        writer.println();
        writer.println("# graf");
        ArrayList<Agent> pom;
        for(int i=0;i<liczbaAgentów;++i){
            writer.print(agenci[i].getPesel()+" ");
            pom=agenci[i].getZnajomi();
            for(Agent temp: pom){
                writer.format("%d ", temp.getPesel());
            }
            writer.println();
        }
        writer.println();
        writer.println("# liczność w kolejnych dniach");
        writer.println((liczbaAgentów-1)+" 1 0");
        writer.flush();
    }
}
