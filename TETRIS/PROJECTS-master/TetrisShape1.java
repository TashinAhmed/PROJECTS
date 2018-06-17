/**
 * Created by vectorpc on 02-Jun-17.
 */
import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.Timer;

public class TetrisShape1 implements ActionListener {
    public int x1,x2,x3,y1,y2,y3;
    public int offSet,offSet2,dropDownTo;
    public boolean onSide;
    public static int num=0;

    Timer time;

    public TetrisShape1(){
        onSide=true;
        if(TetrisPanel.spin!=0){
            TetrisPanel.spin=0;}
        time = new Timer(50,this);
        time.stop();// elements 6 7 8
        coordinates();
    }
    public void coordinates(){

        //onSide=true;
        x1=120;
        x2=140;
        x3=160;
        y1=60;
        y2=60;
        y3=60;
        this.dropDownTo=500;

        // bug fix for spin
        if(TetrisPanel.spin==1){
            TetrisPanel.spin=2;}
        if(TetrisPanel.spin==3){
            TetrisPanel.spin=0;}

        if(onSide==false){
            this.onSide=true;
        }
        TetrisPanel.offSet = this.y2;
        TetrisPanel.offSet2=this.x2;
    }

    public void paint(Graphics g){
        if(onSide==false){

            this.dropDownTo= TetrisPanel.dropDownTo;

            if(y1>=dropDownTo+20){
                TetrisPanel.shapeEnd=true;
                TetrisPanel.num=2;coordinates();}
        }
        if(onSide==true){
            this.dropDownTo= TetrisPanel.dropDownTo;
            if(y1>=dropDownTo+20){
                TetrisPanel.shapeEnd=true;
                TetrisPanel.num=2;coordinates();}
        }
        if(y1<dropDownTo+20){
            TetrisPanel.reached=false;
            y1=y1+1;
            y2=y2+1;
            y3=y3+1;
        }
        if(y1>=this.dropDownTo+20){
            TetrisPanel.reached =true;
        }
        g.setColor(Color.cyan);
        TetrisControl.colr=1;

        g.fillRect(x1, y1, 20, 20);
        g.fillRect(x2, y2, 20, 20);
        g.fillRect(x3, y3, 20, 20);
        checkColumn();
        checkRow();
    }
    public void bar(){
        x1=offSet2;
        x2=offSet2;
        x3=offSet2;						// x1 and y1 are offSet
        y1=offSet+20;
        y2=offSet;
        y3=offSet-20;

    }
    public void barOnItsSide(){
        x1=offSet2-20;
        x2=offSet2;
        x3=offSet2+20;
        y1=offSet;
        y2=offSet;
        y3=offSet;
    }
    public void checkColumn(){
        if(onSide==false){
            switch(x3){
                case 20 : 	TetrisPanel.xNum = 1;break;
                case 40 : 	TetrisPanel.xNum = 2;break;
                case 60 :	TetrisPanel.xNum = 3;break;
                case 80 :	TetrisPanel.xNum = 4;break;
                case 100:	TetrisPanel.xNum = 5;break;
                case 120: 	TetrisPanel.xNum = 6;break;
                case 140: 	TetrisPanel.xNum = 7;break;
                case 160: 	TetrisPanel.xNum = 8;break;
                case 180: 	TetrisPanel.xNum = 9;break;
                case 200: 	TetrisPanel.xNum = 10;break;
                case 220: 	TetrisPanel.xNum = 11;break;
                case 240: 	TetrisPanel.xNum = 12;break;
                case 260: 	TetrisPanel.xNum = 13;break;
                case 280: 	TetrisPanel.xNum = 14;break;
            }
        }
        if(onSide==true){
            switch(x3){
                case 40 : 	TetrisPanel.xNum = 2;break;
                case 60 :	TetrisPanel.xNum = 3;break;
                case 80 :	TetrisPanel.xNum = 4;break;
                case 100:	TetrisPanel.xNum = 5;break;
                case 120: 	TetrisPanel.xNum = 6;break;
                case 140: 	TetrisPanel.xNum = 7;break;
                case 160: 	TetrisPanel.xNum = 8;break;
                case 180: 	TetrisPanel.xNum = 9;break;
                case 200: 	TetrisPanel.xNum = 10;break;
                case 220: 	TetrisPanel.xNum = 11;break;
                case 240: 	TetrisPanel.xNum = 12;break;
                case 260: 	TetrisPanel.xNum = 13;break;
                case 280: 	TetrisPanel.xNum = 14;break;
            }
        }

    }
    public void checkRow(){


        switch(y1){

            case 40 : 	TetrisPanel.yNum = 2;break;
            case 60 :	TetrisPanel.yNum = 3;break;
            case 80 :	TetrisPanel.yNum = 4;break;
            case 100:	TetrisPanel.yNum = 5;break;
            case 120: 	TetrisPanel.yNum = 6;break;
            case 140: 	TetrisPanel.yNum = 7;break;
            case 160: 	TetrisPanel.yNum = 8;break;
            case 180: 	TetrisPanel.yNum = 9;break;
            case 200: 	TetrisPanel.yNum = 10;break;
            case 220: 	TetrisPanel.yNum = 11;break;
            case 240: 	TetrisPanel.yNum = 12;break;
            case 260: 	TetrisPanel.yNum = 13;break;
            case 280 : 	TetrisPanel.yNum = 14;break;
            case 300 :	TetrisPanel.yNum = 15;break;
            case 320:	TetrisPanel.yNum = 16;break;
            case 340:	TetrisPanel.yNum = 17;break;
            case 360: 	TetrisPanel.yNum = 18;break;
            case 380: 	TetrisPanel.yNum = 19;break;
            case 400: 	TetrisPanel.yNum = 20;break;
            case 420: 	TetrisPanel.yNum = 21;break;
            case 440: 	TetrisPanel.yNum = 22;break;
            case 460: 	TetrisPanel.yNum = 23;break;
            case 480: 	TetrisPanel.yNum = 24;break;
            case 500: 	TetrisPanel.yNum = 25;break;
            case 520: 	TetrisPanel.yNum = 26;break;
        }


    }
    @Override
/*
 *
 *
 * (non-Javadoc)
 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
 */
    public void actionPerformed(ActionEvent e) {
        // TODO Auto-generated method stub



    }
}
