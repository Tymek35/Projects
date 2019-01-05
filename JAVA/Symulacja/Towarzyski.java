package symulacja;

import java.util.ArrayList;
import java.util.Random;

public class Towarzyski extends Agent {

    public Towarzyski(int p, int liczbaDni){
        super(p, liczbaDni);
    }
    
    @Override
    public void losujSpotknie(int dzień) {
        //Stała pozwala wyznaczyć dzień na spotkanie, który dopiero nadejdzie
        if(zdrowy==false){
            int s=znajomi.size()-1;
            final int stała=spotkania.length-dzień;
            while(r.nextDouble()<=prawdSpotkania){
                spotkania[r.nextInt(stała)+dzień]=znajomi.get(r.nextInt(s));
            }
        }
        else{
            int s=znajomi.size()-1;
            final int stała=spotkania.length-dzień;
            while(r.nextDouble()<=prawdSpotkania){
                //z prawdopodobieństwem 1/2 wybiera swoich znajomych albo znajomych znajomych
                if(r.nextDouble()<=0.5) spotkania[r.nextInt(stała)+dzień]=znajomi.get(r.nextInt(s));
                else{
                    ArrayList<Agent> pom=znajomi.get(r.nextInt(s)).getZnajomi();
                    spotkania[r.nextInt(stała)+dzień]=pom.get(r.nextInt(pom.size()-1));
                }
            }
        }
    }
    
}
