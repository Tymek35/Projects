package wektory;

import java.util.concurrent.Semaphore;

public class Wektor {

    private final int[] wartości;
    private int iloczyn_skalarny;
    private final Semaphore s;

    private class ObliczanieDodawań implements Runnable {

        private final int start;
        private final int end;
        private final int[] w;
        private final int[] v;
        private final int[] wynik;

        ObliczanieDodawań(int s, int e, int[] w1, int[] w2, int[] wyn) {
            start = s;
            end = e;
            w = w1;
            v = w2;
            wynik = wyn;
        }

        @Override
        public void run() {
            for (int i = start; i < end; ++i) {
                wynik[i] = w[i] + v[i];
            }
        }

    }

    private class IloczynSkalarny implements Runnable {

        private final int start;
        private final int end;
        private final int[] w;
        private final int[] v;

        IloczynSkalarny(int s, int e, int[] w1, int[] w2) {
            start = s;
            end = e;
            w = w1;
            v = w2;
        }

        @Override
        public void run() {
            int część = 0;
            for (int i = start; i < end; ++i) {
                część = część + w[i] * v[i];
            }
            try {
                s.acquire();
            } catch (InterruptedException ex) {
                System.out.println("InterruptedException");
                System.exit(1);
            }
            iloczyn_skalarny = iloczyn_skalarny + część;
            s.release();
        }

    }

    Wektor(int[] war) {
        wartości = new int[war.length];
        for (int i = 0; i < war.length; ++i) {
            wartości[i] = war[i];
        }
        iloczyn_skalarny = 0;
        s = new Semaphore(1);
    }

    public Wektor dodaj(Wektor dod) {
        int[] wynik = new int[wartości.length];
        Thread[] obl = new Thread[wartości.length / 10 + 1];
        int i, j;

        for (i = 0, j = 0; i < wartości.length / 10; ++i, j = j + 10) {
            obl[i] = new Thread(new ObliczanieDodawań(j, j + 10, wartości, dod.wartości, wynik));
            obl[i].start();
        }
        obl[i] = new Thread(new ObliczanieDodawań(j, wartości.length, wartości, dod.wartości, wynik));
        obl[i].start();

        for (i = 0; i < obl.length; ++i) {
            try {
                obl[i].join();
            } catch (InterruptedException ex) {
                System.out.println("Exception");
                System.exit(1);
            }
        }

        return new Wektor(wynik);
    }

    public int iloczSkal(Wektor il) {
        iloczyn_skalarny = 0;
        Thread[] obl = new Thread[wartości.length / 10 + 1];
        int i, j;

        for (i = 0, j = 0; i < wartości.length / 10; ++i, j = j + 10) {
            obl[i] = new Thread(new IloczynSkalarny(j, j + 10, wartości, il.wartości));
            obl[i].start();
        }
        obl[i] = new Thread(new IloczynSkalarny(j, wartości.length, wartości, il.wartości));
        obl[i].start();

        for (i = 0; i < obl.length; ++i) {
            try {
                obl[i].join();
            } catch (InterruptedException ex) {
                System.out.println("InterruptedException");
                System.exit(1);
            }
        }

        return iloczyn_skalarny;
    }

    @Override
    public String toString() {
        String s = new String();
        for (int i = 0; i < wartości.length; ++i) {
            s = s + " " + wartości[i];
        }
        return s;
    }
}
