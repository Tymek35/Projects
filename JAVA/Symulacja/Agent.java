package symulacja;

import java.util.ArrayList;
import java.util.Random;

public abstract class Agent {
    protected static double prawdWyzdrowienia;
    protected static double śmiertelność;
    protected static double prawdZarażenia;
    protected static double prawdSpotkania;
    protected static int seed;
    protected ArrayList<Agent> znajomi;
    //Przyjmuję, że dany agent może się umówić na jedno spotkanie w danym dniu
    protected Agent[] spotkania;
    protected boolean żywy;
    protected boolean zdrowy;
    protected boolean odporny;
    protected int pesel;//Unikalny numer przypisany agentowi
    protected Random r;
    
    public abstract void losujSpotknie(int dzień);
    
    public void spotkajSię(int dzień){
        if(spotkania[dzień]!=null && spotkania[dzień].getŻywy()){
            if(zdrowy==true && spotkania[dzień].getZdrowy()==false){
                if(r.nextDouble()<=prawdZarażenia) zachoruj();
            }
            else if(zdrowy==false && spotkania[dzień].getZdrowy()==true){
                if(r.nextDouble()<=prawdZarażenia) spotkania[dzień].zachoruj();
            }
        }
    }
    
    public void nastDzień(){
        if(zdrowy==false){
            if(r.nextDouble()<=prawdWyzdrowienia){
                zdrowy=true;
                odporny=true;
            }
            else{
                if(r.nextDouble()<=śmiertelność){
                    żywy=false;
                    for(Agent pom:znajomi){
                        pom.usuńZnajomego(this);
                    }
                }
            }
        }
    }
    
    public Agent(int p, int liczbaDni){
        żywy=true;
        zdrowy=true;
        odporny=false;
        pesel=p;
        znajomi=new ArrayList<Agent>();
        spotkania=new Agent[liczbaDni];
        for(int i=0;i<liczbaDni;++i){
            spotkania[i]=null;
        }
        r=new Random(seed);
    }
    
    public boolean getŻywy(){
        return żywy;
    }
    
    public boolean getZdrowy(){
        return zdrowy;
    }
    
    public boolean getOdporny(){
        return odporny;
    }
    
    public int getPesel(){
        return pesel;
    }
    
    public void zachoruj(){
        if(odporny==false) zdrowy=false;
    }
    
    public ArrayList<Agent> getZnajomi(){
        return znajomi;
    }
    
    public static void setStałe(double pw, double ś, double pz, double ps, int s){
        prawdWyzdrowienia=pw;
        śmiertelność=ś;
        prawdZarażenia=pz;
        prawdSpotkania=ps;
        seed=s;
    }
    
    public void dodajZnajomego(Agent a){
        znajomi.add(a);
    }
    
    public void usuńZnajomego(Agent a){
        znajomi.remove(a);
    }
    
    public boolean czyMaWZnajomych(Agent a){
        return znajomi.contains(a);
    }
    
    public boolean equals(Agent a){
        return pesel==a.getPesel();
    }
    
}
