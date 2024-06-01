import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import javax.swing.JPanel;
import java.util.ArrayList;

public class VueTrain extends JPanel {
    private final Train train;

    public VueTrain(Train train) {
        setOpaque(false);
        this.train = train;
        setPreferredSize(new Dimension(getWidth(), 3*getHeight()/4));
    }


    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        setOpaque(false);

        int wagonWidth = getWidth() / Train.NB_WAGONS;
        int butinYOffset = getHeight() / 2; // Commencer à dessiner les butins à la moitié de la hauteur du wagon

        for (int i = 0; i < Train.NB_WAGONS; i++) {//dessin des wagons
            int x = i * wagonWidth;
            g.setColor(Color.BLACK);
            g.drawRect(x, 0, wagonWidth, getHeight() / 2);
            g.drawString("Wagon " + (i + 1), x + 10, getHeight() / 2 - 10);


            Wagon wagon = train.getWagons()[i];
            ArrayList<Butin> butins = wagon.getButins();
            int butinXOffset = 10; // Un peu d'espacement depuis le bord gauche du wagon

            for (int j = 0; j < butins.size(); j++) {
                Butin butin = butins.get(j);
                String butinText = getButinType(butin) + ": " + butin.getValeur();
                g.drawString(butinText, x + butinXOffset, butinYOffset + (j + 1) * 15); // Décaler chaque butin de 15 pixels en Y
            }
        }
    }

    private String getButinType(Butin butin) {
        return switch (butin) {
            case Bourse bourse -> "Bourse";
            case Bijou bijou -> "Bijou";
            case Magot magot -> "Magot";
            case MontreDeLuxe montreDeLuxe -> montreDeLuxe.getModele();
            case null, default -> "Inconnu";
        };
    }

}