package bot;

import com.sun.glass.events.KeyEvent;
import java.awt.AWTException;
import java.awt.Robot;
import java.util.Random;

//Simple program which pushes random buttons what
//stops kicking player of the server in GTA 5 for
//being inactive. 

public class Bot {

    public static void main(String[] args) {
        try {
            Robot r=new Robot();
            Random random=new Random();
            while(true){
                switch(random.nextInt(4)){
                    case 0:
                        r.keyPress(KeyEvent.VK_W);
                        r.delay(random.nextInt(1000));
                        r.keyRelease(KeyEvent.VK_W);
                        break;
                    case 1:
                        r.keyPress(KeyEvent.VK_S);
                        r.delay(random.nextInt(1000));
                        r.keyRelease(KeyEvent.VK_S);
                        break;
                    case 2:
                        r.keyPress(KeyEvent.VK_A);
                        r.delay(random.nextInt(1000));
                        r.keyRelease(KeyEvent.VK_A);
                        break;
                    case 3:
                        r.keyPress(KeyEvent.VK_D);
                        r.delay(random.nextInt(1000));
                        r.keyRelease(KeyEvent.VK_D);
                        break;
                }
                r.delay(random.nextInt(10000));
            }
        } catch (AWTException ex) {
            System.out.println("AWTException");
            System.exit(1);
        }
    }
    
}
