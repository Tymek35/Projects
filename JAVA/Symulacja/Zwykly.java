package symulacja;

import java.util.Random;

public class Zwykly extends Agent {

    public Zwykly(int p, int liczbaDni){
        super(p, liczbaDni);
    }
    
    @Override
    public void losujSpotknie(int dzień) {
        //Stała pozwala wyznaczyć dzień na spotkanie, który dopiero nadejdzie
        if(zdrowy==true){
            int s=znajomi.size()-1;
            final int stała=spotkania.length-dzień;
            while(r.nextDouble()<=prawdSpotkania){
                spotkania[r.nextInt(stała)+dzień]=znajomi.get(r.nextInt(s));
            }
        }
        else{
            int s=znajomi.size()-1;
            final double nowePrawdSpotkania=prawdSpotkania*0.5;
            final int stała=spotkania.length-dzień;
            while(r.nextDouble()<=nowePrawdSpotkania){
                spotkania[r.nextInt(stała)+dzień]=znajomi.get(r.nextInt(s));
            }
        }
    }
    
}
