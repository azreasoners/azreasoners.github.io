// Written by Tae-Won Kim
// Updated on 11/10/2008

import javax.swing.*; // to use JApplet

public class TutoringSystem extends JApplet
{
	//private final int APPLET_W = 500, APPLET_H = 550;

	public void init()
	{
		SystemPanel p = new SystemPanel();
		getContentPane().add(p);
		//setSize(APPLET_W, APPLET_H);
	}
}

