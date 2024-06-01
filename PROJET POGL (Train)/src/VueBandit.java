import javax.swing.*;
import java.awt.*;

public class VueBandit extends JPanel implements observer {
    private final Bandit[] bandit;
    private final Color[] couleursBandits = {Color.RED, Color.BLACK, Color.GREEN, Color.YELLOW};


    public VueBandit(Bandit[] bandit) {
        this.bandit = bandit;
        for (Bandit value : bandit) {
            value.addObserver(this);
        }
        setPreferredSize(new Dimension(400, 100));
        setOpaque(false);
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        // Calcul de la largeur d'un wagon en fonction de la largeur totale de la vue
        int wagonWidth = getWidth() / (Train.NB_WAGONS);
        int wagonHeight = getHeight();
        int banditWidth = 20;
        int banditHeight = 20;
        setOpaque(false);
        for (int i = 0; i < bandit.length; i++) {
            // Position en X du bandit : dépend du wagon sur lequel il se trouve
            int x = (bandit[i].getPositionWagon() - 1) * wagonWidth;

            // Position en Y du bandit : dépend de s'il est sur le toit ou à l'intérieur
            int y = bandit[i].isSurToit() ? i*banditHeight : wagonHeight / 2 - banditHeight+20+ i*banditHeight;

            // Dessin du bandit
            g.setColor(couleursBandits[i]);
            g.fillOval(x + wagonWidth / 2 - banditWidth / 2, y, banditWidth, banditHeight);

            // Dessin du nom du bandit ,
            g.setColor(Color.BLACK);
            g.drawString(bandit[i].getNom(), x + wagonWidth / 2 + banditWidth/2, y + 15);
            bandit[i].addObserver(this);
        }
    }


    @Override
    public void update() {
        repaint();
    }
}
