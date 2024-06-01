import javax.swing.*;
import java.awt.*;
import java.util.Objects;
import java.util.Random;

public class VueCouleur extends JPanel {
    private Image cloudImage;
    private final Image sunImage;
    private final Image cactusImage;
    private final Image cactus2Image;

    public VueCouleur() {
        ImageIcon icon = new ImageIcon(Objects.requireNonNull(getClass().getResource("nuage.png")));
        cloudImage = icon.getImage();
        int width = cloudImage.getWidth(this) * 20; // Agrandir l'image en largeur x20
        int height = cloudImage.getHeight(this) * 20; // Agrandir l'image en hauteur x20
        cloudImage = cloudImage.getScaledInstance(width, height, Image.SCALE_SMOOTH);
        ImageIcon icon2 =new ImageIcon(Objects.requireNonNull(getClass().getResource("soleil.png")));
        sunImage=icon2.getImage();

        ImageIcon icon3=new ImageIcon(Objects.requireNonNull(getClass().getResource("cactus.png")));
        cactusImage=icon3.getImage();

        ImageIcon icon4 = new ImageIcon(Objects.requireNonNull(getClass().getResource("cactus2.png")));
        cactus2Image=icon4.getImage();
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        g.setColor(new Color(135,206,250));
        g.fillRect(0, 0, this.getWidth(), 200); //ciel
        Random rand=new Random();
        int x = rand.nextInt(getWidth()); // Centrer le nuage en largeur
        int y = (150 - cloudImage.getHeight(this)) / 2; // Centrer le nuage en hauteur dans le ciel
        g.drawImage(cloudImage, x, y, this);

        int x2 = rand.nextInt(getWidth()); // Centrer le nuage en largeur
        g.drawImage(cloudImage, x2, y, this);

        g.drawImage(sunImage,0,0,this);

        int xCactus=rand.nextInt(getWidth());
        int yCactus=rand.nextInt(200);

        int x2Cactus=rand.nextInt(getWidth());
        int y2Cactus=rand.nextInt(200);

        g.setColor(new Color(150, 75, 0));
        g.fillRect(0, 200, this.getWidth(), 100); //train

        g.setColor(new Color(255, 203, 96));
        g.fillRect(0, 300, this.getWidth(), this.getHeight() - 300); //sable
        g.drawImage(cactusImage,xCactus,400+yCactus,this);
        g.drawImage(cactus2Image,x2Cactus,400+y2Cactus,this);
    }
}
