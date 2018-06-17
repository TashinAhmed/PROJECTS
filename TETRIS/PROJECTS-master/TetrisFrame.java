/**
 * Created by vectorpc on 02-Jun-17.
 */


import java.awt.Dimension;
import java.awt.Toolkit;



import javax.swing.JFrame;



public class TetrisFrame extends JFrame{

    private static final long serialVersionUID = 1L;
    public TetrisPanel pan;
    public Dimension dim;
    int locX;
    int locY;
    public TetrisFrame() throws InterruptedException{

        dim = Toolkit.getDefaultToolkit().getScreenSize();
        locX=(int) dim.getWidth()*4/12;//this is to set the game into the center of the screen
        locY=(int) dim.getHeight()*2/12;
        pan = new TetrisPanel();
        this.setTitle("TretisFrame");
        this.setSize(326,585);
        this.setLocation(locX, locY);

        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setVisible(true);
        this.setFocusable(true);
        this.setResizable(false);
        this.add(pan);
        addKeyListener(pan);

    }



    public static void main(String[] args) throws InterruptedException{
        new TetrisFrame();
    }
}

