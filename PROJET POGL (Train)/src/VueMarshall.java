import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import javax.swing.JPanel;

public class VueMarshall extends JPanel {
    private final Marshall marshall;

    public VueMarshall(Marshall marshall) {
        this.marshall=marshall;
        setPreferredSize(new Dimension(800, 600));
        setOpaque(false);

    }
    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        setOpaque(false);
        int wagonWidth = getWidth() / Train.NB_WAGONS;
        int marshallPosition = marshall.getPositionWagon();
        int xMarshall = (marshallPosition - 1) * wagonWidth + wagonWidth / 2;
        int yMarshall = getHeight() / 4;
        g.setColor(Color.BLUE);
        g.fillOval(xMarshall, yMarshall, 20, 20);
        g.setColor(Color.BLACK);
        g.drawString("Marshall", xMarshall - 15, yMarshall - 5);
    }
}