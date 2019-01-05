package symulacja;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.nio.charset.Charset;
import java.util.Properties;

public class Wczytywania {
    private int seed;
    private int liczbaAgentów;
    private int liczbaDni;
    private double prawdTowarzyski;
    private int średniaLiczbaZnajomych;
    private PrintWriter writer;
    private double prawdWyzdrowienia;
    private double śmiertelność;
    private double prawdZarażenia;
    private double prawdSpotkania;
    
    public Wczytywania() {
        Properties xml=new Properties();
        try {
            FileInputStream input1=new FileInputStream("simulation-conf.xml");
            xml.loadFromXML(input1);
        } catch (FileNotFoundException ex) {
            System.out.println("Brak pliku simulation-conf.xml");
            System.exit(1);
        } catch(IOException ex){
            System.out.println("Plik simulation-conf nie jest xml.");
            System.exit(1);
        }
        Properties defaultProp=new Properties();
        try {
            FileInputStream input2=new FileInputStream("default.properties.txt");
            defaultProp.load(new InputStreamReader(input2, Charset.forName("UTF-8")));
        } catch (FileNotFoundException ex) {
            System.out.println("Brak pliku default.properties");
            System.exit(1);
        } catch(IOException ex){
            System.out.println("Plik default.properties nie jest txt.");
            System.exit(1);
        }
        wczytajZXML(xml);
        wczytajZDefault(defaultProp);
    }
    
    private void wczytajZXML(Properties p){
        //Jeżeli jakiejś danej nie ma w tym pliku to wpisuję tam tymczasowo -1 lub null
        String s="", klucz="";
        try {
            klucz="seed";
            s=p.getProperty(klucz, "-1");
            seed=Integer.parseInt(s);
            klucz="liczbaAgentów";
            s=p.getProperty(klucz, "-1");
            liczbaAgentów=Integer.parseInt(s);
            klucz="liczbaDni";
            s=p.getProperty(klucz, "-1");
            liczbaDni=Integer.parseInt(s);
            klucz="śrZnajomych";
            s=p.getProperty(klucz, "-1");
            średniaLiczbaZnajomych=Integer.parseInt(s);
            klucz="śmiertelność";
            s=p.getProperty(klucz, "-1.0");
            śmiertelność=Double.parseDouble(s);
            klucz="prawdTowarzyski";
            s=p.getProperty(klucz, "-1.0");
            prawdTowarzyski=Double.parseDouble(s);
            klucz="prawdWyzdrowienia";
            s=p.getProperty(klucz, "-1.0");
            prawdWyzdrowienia=Double.parseDouble(s);
            klucz="prawdZarażenia";
            s=p.getProperty(klucz, "-1.0");
            prawdZarażenia=Double.parseDouble(s);
            klucz="prawdSpotkania";
            s=p.getProperty(klucz, "-1.0");
            prawdSpotkania=Double.parseDouble(s);
            klucz="plikZRaportem";
            s=p.getProperty(klucz, "");
            if(s.length()!=0){
                try {
                    writer=new PrintWriter(new File(s));
                } catch (FileNotFoundException ex) {
                    System.out.println("Brak wartości dla klucza plikZRaportem");
                    System.exit(1);
                }
            }
            else writer=null;
        } catch(NumberFormatException ex) {
            System.out.println("Niedozwolona wartość \""+s+"\" dla klucza "+klucz);
            System.exit(1);
        }
    }
    
    private void wczytajZDefault(Properties p){
        //W pliku default nie ma polskich znaków.
        try {
            if(seed==-1) seed=Integer.parseInt(p.getProperty("seed"));
            if(liczbaAgentów==-1) liczbaAgentów=Integer.parseInt(p.getProperty("liczbaAgentow"));
            if(liczbaDni==-1) liczbaDni=Integer.parseInt(p.getProperty("liczbaDni"));
            if(średniaLiczbaZnajomych==-1) średniaLiczbaZnajomych=Integer.parseInt(p.getProperty("srZnajomych"));
            if(śmiertelność==-1.0) śmiertelność=Double.parseDouble(p.getProperty("smiertelnosc"));
            if(prawdTowarzyski==-1.0) prawdTowarzyski=Double.parseDouble(p.getProperty("prawdTowarzyski"));
            if(prawdWyzdrowienia==-1.0) prawdWyzdrowienia=Double.parseDouble(p.getProperty("prawdWyzdrowienia"));
            if(prawdZarażenia==-1.0) prawdZarażenia=Double.parseDouble(p.getProperty("prawdZarazenia"));
            if(prawdSpotkania==-1.0) prawdSpotkania=Double.parseDouble(p.getProperty("prawdSpotkania"));
            if(writer==null){
                try {
                    writer=new PrintWriter(new File(p.getProperty("plikZRaportem")));
                } catch (FileNotFoundException ex) {
                    System.out.println("Brak wartości dla klucza plikZRaportem");
                    System.out.println("Najpewniej problem z polskimi znakami");
                    System.exit(1);
                }
            }
        } catch(NumberFormatException ex) {
            System.out.println("Błąd w pliku default.properties");
            System.exit(1);
        }
        if(liczbaAgentów<=średniaLiczbaZnajomych){
            System.out.println("Za duża średnia liczba znajomych w porównaniu z liczbą agentów");
            System.exit(1);
        }
    }
    
    public void ustawAgentów(){
        Agent.setStałe(prawdWyzdrowienia, śmiertelność, prawdZarażenia, prawdSpotkania, seed);
    }
    
    public Epidemia przygotujSymulację(){
        return new Epidemia(seed, liczbaAgentów, liczbaDni, prawdTowarzyski, writer, średniaLiczbaZnajomych);
    }
}
