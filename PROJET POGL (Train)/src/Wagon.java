import java.util.ArrayList;
import java.util.Random;

// Classe représentant un wagon du train avec des butins
public class Wagon {
    private final ArrayList<Butin> butins;
    public Wagon(int numero) {
        this.butins = new ArrayList<>();
        if (numero == 1) {
            this.butins.add(new Magot());  //si locomotive, on met magot
        } else {
            genererButinsAleatoires();
        }
        if(numero==2){//on met la montre en premiere classe
            genererButinsAleatoiresMontre(this);
        }
    }

    private void genererButinsAleatoires() {
        Random rand = new Random();
        int nbButins = rand.nextInt(4) + 1; // Entre 1 et 4 butins
        for (int i = 0; i < nbButins; i++) {
            int type = rand.nextInt(2); // 0 pour bourse, 1 pour bijou
            if (type == 0) {
                butins.add(new Bourse());
            } else {
                butins.add(new Bijou());
            }
        }
    }
    private void genererButinsAleatoiresMontre(Wagon w) {
        Random rand = new Random();
        int chance = rand.nextInt(100); // Tirage entre 0 et 99
        if (chance < 40) {
            w.butins.add(new MontreDeLuxe("Tissot Couturier", 4000));
        } else if (chance < 70) {
            w.butins.add(new MontreDeLuxe("Raymond Weil", 3500));
        } else if (chance < 90) {
            w.butins.add(new MontreDeLuxe("Rolex Datejust", 6000));
        } else {
            w.butins.add(new MontreDeLuxe("Rolex Daytona", 25000));
        }
    }
    public ArrayList<Butin> getButins() {
        return butins;
    }
}