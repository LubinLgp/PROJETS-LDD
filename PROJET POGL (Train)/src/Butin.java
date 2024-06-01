import java.util.Random;

// Classe abstraite représentant un butin
public abstract class Butin {
    abstract int getValeur();
}

// Classe représentant une bourse
class Bourse extends Butin {
    private final int valeur;

    public Bourse() {
        Random rand = new Random();
        this.valeur = rand.nextInt(501); // Valeur entre 0 et 500
    }

    @Override
    int getValeur() {
        return valeur;
    }
}

// Classe représentant un bijou
class Bijou extends Butin {
    private static final int VALEUR_BIJOU = 500;

    @Override
    int getValeur() {
        return VALEUR_BIJOU;
    }
}

// Classe représentant un magot
class Magot extends Butin {
    private static final int VALEUR_MAGOT = 1000;

    @Override
    int getValeur() {
        return VALEUR_MAGOT;
    }
}
class MontreDeLuxe extends Butin {
    private final String modele;
    private final int valeur;

    public MontreDeLuxe(String modele, int valeur) {
        this.modele = modele;
        this.valeur = valeur;
    }

    @Override
    int getValeur() {
        return this.valeur;
    }

    public String getModele() {
        return this.modele;
    }
}
