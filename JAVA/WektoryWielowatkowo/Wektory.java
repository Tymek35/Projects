package wektory;

import java.util.Random;

//Program liczy sumy i iloczyny skalarne wektorow wielowatkowo.

public class Wektory {

    public static void main(String[] args) {
        int dł = 2, max=4;
        Random r = new Random();
        int[] w1 = new int[dł];
        int[] w2 = new int[dł];
        for (int i = 0; i < w1.length; ++i) {
            w1[i] = r.nextInt(max);
            w2[i] = r.nextInt(max);
        }
        Wektor w = new Wektor(w1);
        Wektor v = new Wektor(w2);
        Wektor z = w.dodaj(v);

        System.out.println(w);
        System.out.println(v);
        System.out.println(z);
        System.out.println(w.iloczSkal(v));
    }

}
