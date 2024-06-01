import javax.swing.*;
import java.awt.*;

// Classe représentant la vue principale du jeu
public class Vue extends JFrame {
    public Vue(Train train, Bandit[] bandit,Marshall marshall) {

        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setTitle("Colt Express");

        // Création du JLayeredPane pour superposer les vues


        JLayeredPane layeredPane = new JLayeredPane();
        layeredPane.setPreferredSize(new Dimension(800, 800));

        VueCouleur vueCouleur = new VueCouleur();
        vueCouleur.setBounds(0, 0, 800, 800);
        layeredPane.add(vueCouleur, Integer.valueOf(0)); // couleur en fond

        VueMarshall vueMarshall = new VueMarshall(marshall);
        vueMarshall.setBounds(0, 200, 800, 200);
        layeredPane.add(vueMarshall, Integer.valueOf(2)); // marshall au niveau le plus haut

        VueBandit vueBandit = new VueBandit(bandit);
        vueBandit.setBounds(0, 120, 800, 200);
        layeredPane.add(vueBandit, Integer.valueOf(2)); //bandit au plus haut aussi
        VueTrain vueTrain = new VueTrain(train);
        vueTrain.setBounds(0, 200, 800, 200);
        layeredPane.add(vueTrain, Integer.valueOf(1)); //train entre fond de couleur et acteurs


        VueButins vueButins = new VueButins(train.getBandits());
        vueButins.setBounds(0, 400, 800, 200);
        layeredPane.add(vueButins, Integer.valueOf(2)); // butins au niveau le plus haut de superposition

        VueCommandes vueCommandes = new VueCommandes(bandit,vueTrain,vueBandit,vueButins,vueMarshall, marshall, train, vueCouleur);
        vueCommandes.setBounds(0, 600, 800, 200);
        layeredPane.add(vueCommandes, Integer.valueOf(2)); // commandes au niveau le plus haut aussi

        // Ajout du JLayeredPane à la JFrame
        setContentPane(layeredPane);
        pack();
        setLocationRelativeTo(null);
        setVisible(true);

    }
}