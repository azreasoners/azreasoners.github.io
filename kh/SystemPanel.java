import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;
import java.io.*;
//import java.security.AccessControlException;


public class SystemPanel extends JPanel implements SwingConstants
{
	private JLabel label, label2;
	private JPanel p1, p2, p2_place, p2_intimacy, p2_sRank, p2_kinship,
	               p2_ageDifference, p2_ageSpeaker, p2_ageListener,
	               p3, p4;
	private JCheckBox formal, informal, distant, close,
	                  position_s_higher, position_l_higher, position_equal, position_noRel,
	                  kindred_s_higher, kindred_l_higher, kindred_equal, kindred_noRel,
	                  s_much_older, s_older, equal, l_much_older, l_older,
	                  s_elder, s_adult, s_young_adult, s_teenager, s_child,
	                  l_elder, l_adult, l_young_adult, l_teenager, l_child;
	private JButton ok, reset;
	private JTextArea ta;
	private String f /*formality*/ ,k /*kinship*/ ,r /*social rank*/,
	               d /*age difference*/ ,s /*Speaker's age*/,
	               l /*Listener's age*/ ,i;/*intimacy*/
	private int f_count, k_count, r_count, d_count, s_count, l_count, i_count;
	private boolean writing;

	public SystemPanel()
	{
		setLayout (new BoxLayout (this, BoxLayout.Y_AXIS));

		p1 = new JPanel();
		p1.setLayout (new GridLayout(2,1));
		label = new JLabel("What speech level I can use is...");
		label.setFont(new Font("Dialog", Font.PLAIN, 20));
		label.setForeground(Color.red);
		p1.add(label);
		p1.add(new JLabel (""));
		add(p1);


		p2 = new JPanel();
	    p2.setLayout (new GridLayout(4,2));

 		// Place
	    p2_place = new JPanel();
	    p2_place.setLayout (new BoxLayout (p2_place, BoxLayout.X_AXIS));
	    TitledBorder tb = BorderFactory.createTitledBorder("Place");
	    p2_place.setBorder(tb);
        formal = new JCheckBox("Formal");
		informal = new JCheckBox("Informal");
		FormalityListener listener = new FormalityListener();
		formal.addItemListener(listener);
		informal.addItemListener(listener);
		p2_place.add(formal);
		p2_place.add(informal);
		p2.add(p2_place);

		// Degree of Kinship
		p2_kinship = new JPanel();
		p2_kinship.setLayout (new BoxLayout (p2_kinship, BoxLayout.X_AXIS));
		TitledBorder tb2 = BorderFactory.createTitledBorder("Degree of Kinship: S(Speaker) / L(Listener)");
		p2_kinship.setBorder(tb2);
		kindred_s_higher = new JCheckBox("Higher(S)");
		kindred_l_higher = new JCheckBox("Higher(L)");
		kindred_equal = new JCheckBox("Equal");
		kindred_noRel = new JCheckBox("No relationship");
		KindredListener listener2 = new KindredListener();
		kindred_s_higher.addItemListener(listener2);
		kindred_l_higher.addItemListener(listener2);
		kindred_equal.addItemListener(listener2);
		kindred_noRel.addItemListener(listener2);
		p2_kinship.add(kindred_s_higher);
		p2_kinship.add(kindred_l_higher);
		p2_kinship.add(kindred_equal);
		p2_kinship.add(kindred_noRel);
		p2.add(p2_kinship);

		// Social rank (position)
		p2_sRank = new JPanel();
		p2_sRank.setLayout (new BoxLayout (p2_sRank, BoxLayout.X_AXIS));
		TitledBorder tb3 = BorderFactory.createTitledBorder("Social Rank: S(Speaker) / L(Listener)");
		p2_sRank.setBorder(tb3);
		position_s_higher = new JCheckBox("Higher(S)");
		position_l_higher = new JCheckBox("Higher(L)");
		position_equal = new JCheckBox("Equal");
		position_noRel = new JCheckBox("No relationship");
		PositionListener listener3 = new PositionListener();
		position_s_higher.addItemListener(listener3);
		position_l_higher.addItemListener(listener3);
		position_equal.addItemListener(listener3);
		position_noRel.addItemListener(listener3);
		p2_sRank.add(position_s_higher);
		p2_sRank.add(position_l_higher);
		p2_sRank.add(position_equal);
		p2_sRank.add(position_noRel);
		p2.add(p2_sRank);

		// Age Difference
		p2_ageDifference = new JPanel();
		p2_ageDifference.setLayout (new BoxLayout (p2_ageDifference, BoxLayout.X_AXIS));
		TitledBorder tb4 = BorderFactory.createTitledBorder("Age Difference: S(Speaker) / L(Listener)");
		p2_ageDifference.setBorder(tb4);
		s_much_older = new JCheckBox("Much older(S)");
		s_older = new JCheckBox("Older(S)");
		equal = new JCheckBox("Equal");
		l_much_older = new JCheckBox("Much older(L)");
		l_older = new JCheckBox("Older(L)");
		AgeDifferenceListener listener4 = new AgeDifferenceListener();
		s_much_older.addItemListener(listener4);
		s_older.addItemListener(listener4);
		equal.addItemListener(listener4);
		l_much_older.addItemListener(listener4);
		l_older.addItemListener(listener4);
		p2_ageDifference.add(s_much_older);
		p2_ageDifference.add(s_older);
		p2_ageDifference.add(equal);
		p2_ageDifference.add(l_much_older);
		p2_ageDifference.add(l_older);
		p2.add(p2_ageDifference);

		// Speaker's age
		p2_ageSpeaker = new JPanel();
		p2_ageSpeaker.setLayout (new GridLayout(2,3));
		TitledBorder tb5 = BorderFactory.createTitledBorder("Speaker's age (years old)");
		p2_ageSpeaker.setBorder(tb5);
		s_elder = new JCheckBox("Elder(60~)");
		s_adult = new JCheckBox("Adult(40~60)");
		s_young_adult = new JCheckBox("Y_adult(20~40)");
		s_teenager = new JCheckBox("Teenager(10~20)");
		s_child = new JCheckBox("Child(0~10)");
		SpeakerAgeListener listener5 = new SpeakerAgeListener();
		s_elder.addItemListener(listener5);
		s_adult.addItemListener(listener5);
		s_young_adult.addItemListener(listener5);
		s_teenager.addItemListener(listener5);
		s_child.addItemListener(listener5);
		p2_ageSpeaker.add(s_young_adult);
		p2_ageSpeaker.add(s_adult);
		p2_ageSpeaker.add(s_elder);
		p2_ageSpeaker.add(s_child);
		p2_ageSpeaker.add(s_teenager);
		p2.add(p2_ageSpeaker);

		// Listener's age
		p2_ageListener = new JPanel();
		p2_ageListener.setLayout (new GridLayout(2,3));
		TitledBorder tb6 = BorderFactory.createTitledBorder("Listener's age (years old)");
		p2_ageListener.setBorder(tb6);
		l_elder = new JCheckBox("Elder(60~)");
		l_adult = new JCheckBox("Adult(40~60)");
		l_young_adult = new JCheckBox("Y_adult(20~40)");
		l_teenager = new JCheckBox("Teenager(10~20)");
		l_child = new JCheckBox("Child(0~10)");
		ListenerAgeListener listener6 = new ListenerAgeListener();
		l_elder.addItemListener(listener6);
		l_adult.addItemListener(listener6);
		l_young_adult.addItemListener(listener6);
		l_teenager.addItemListener(listener6);
		l_child.addItemListener(listener6);
		p2_ageListener.add(l_young_adult);
		p2_ageListener.add(l_adult);
		p2_ageListener.add(l_elder);
		p2_ageListener.add(l_child);
		p2_ageListener.add(l_teenager);
		p2.add(p2_ageListener);

 		// intimacy
	    p2_intimacy = new JPanel();
	    p2_intimacy.setLayout (new BoxLayout (p2_intimacy, BoxLayout.X_AXIS));
	    TitledBorder tb7 = BorderFactory.createTitledBorder("Intimacy");
	    p2_intimacy.setBorder(tb7);
        distant = new JCheckBox("Distant");
		close = new JCheckBox("Close");
		IntimacyListener listener7 = new IntimacyListener();
		distant.addItemListener(listener7);
		close.addItemListener(listener7);
		p2_intimacy.add(distant);
		p2_intimacy.add(close);
		p2.add(p2_intimacy);

		add(p2);


	    p3 = new JPanel();
		ok = new JButton("OK");
		reset=new JButton("Reset");
		ok.addActionListener (new ButtonListener());
		reset.addActionListener (new ButtonListener());
		p3.add(ok);
		p3.add(reset);
		add(p3);


		p4 = new JPanel();
		ta = new JTextArea("Have Fun!!!", 15, 50); // set size
		p4.add(ta);
		add(p4);
	}

	private class FormalityListener implements ItemListener
	{
		public void itemStateChanged (ItemEvent event)
		{
			f_count=0;
			if(formal.isSelected())
			{
				f="formality(speaker,listener,formal).";
				f_count++;
			}
			if(informal.isSelected())
			{
				f="formality(speaker,listener,informal).";
				f_count++;
			}
		}
	}

	private class KindredListener implements ItemListener
	{
		public void itemStateChanged (ItemEvent event)
		{
			k_count=0;
			if(kindred_s_higher.isSelected())
			{
				k="kindred(speaker,listener,k_higher).";
				k_count++;
			}
			if(kindred_l_higher.isSelected())
			{
				k="kindred(listener,speaker,k_higher).";
				k_count++;
			}
			if(kindred_equal.isSelected())
			{
				k="kindred(speaker,listener,k_equal).";
				k_count++;
			}
			if(kindred_noRel.isSelected())
			{
				k="kindred(speaker,listener,k_noRel).";
				k_count++;
			}
		}
	}

	private class PositionListener implements ItemListener
	{
		public void itemStateChanged (ItemEvent event)
		{
			r_count=0;
			if(position_s_higher.isSelected())
			{
				r="position(speaker,listener,p_higher).";
				r_count++;
			}
			if(position_l_higher.isSelected())
			{
				r="position(listener,speaker,p_higher).";
				r_count++;
			}
			if(position_equal.isSelected())
			{
				r="position(speaker,listener,p_equal).";
				r_count++;
			}
			if(position_noRel.isSelected())
			{
				r="position(speaker,listener,p_noRel).";
				r_count++;
			}
		}
	}

	private class AgeDifferenceListener implements ItemListener
	{
		public void itemStateChanged (ItemEvent event)
		{
			d_count=0;
			if(s_much_older.isSelected())
			{
				d="older(speaker,listener,much_older).";
				d_count++;
			}
			if(s_older.isSelected())
			{
				d="older(speaker,listener,older).";
				d_count++;
			}
			if(equal.isSelected())
			{
				d="older(speaker,listener,equal).";
				d_count++;
			}
			if(l_much_older.isSelected())
			{
				d="older(listener,speaker,much_older).";
				d_count++;
			}
			if(l_older.isSelected())
			{
				d="older(listener,speaker,older).";
				d_count++;
			}
		}
	}

	private class SpeakerAgeListener implements ItemListener
	{
		public void itemStateChanged (ItemEvent event)
		{
			s_count=0;
			if(s_elder.isSelected())
			{
				s="age(speaker,elder).";
				s_count++;
			}
			if(s_adult.isSelected())
			{
				s="age(speaker,adult).";
				s_count++;
			}
			if(s_young_adult.isSelected())
			{
				s="age(speaker,young_adult).";
				s_count++;
			}
			if(s_teenager.isSelected())
			{
				s="age(speaker,teenager).";
				s_count++;
			}
			if(s_child.isSelected())
			{
				s="age(speaker,child).";
				s_count++;
			}
		}
	}

	private class ListenerAgeListener implements ItemListener
	{
		public void itemStateChanged (ItemEvent event)
		{
			l_count=0;
			if(l_elder.isSelected())
			{
				l="age(speaker,elder).";
				l_count++;
			}
			if(l_adult.isSelected())
			{
				l="age(speaker,adult).";
				l_count++;
			}
			if(l_young_adult.isSelected())
			{
				l="age(speaker,young_adult).";
				l_count++;
			}
			if(l_teenager.isSelected())
			{
				l="age(speaker,teenager).";
				l_count++;
			}
			if(l_child.isSelected())
			{
				l="age(speaker,child).";
				l_count++;
			}
		}
	}

	private class IntimacyListener implements ItemListener
	{
		public void itemStateChanged (ItemEvent event)
		{
			i_count=0;
			if(distant.isSelected())
			{
				i="intimacy(speaker,listener,distant).";
				i_count++;
			}
			if(close.isSelected())
			{
				i="intimacy(speaker,listener,close).";
				i_count++;
			}
		}
	}

	private class ButtonListener implements ActionListener
	{
		public void actionPerformed (ActionEvent event)
		{
			if(event.getSource()==reset)
		   	{
				formal.setSelected(false); informal.setSelected(false);
	       	    distant.setSelected(false);close.setSelected(false);
	       	    position_s_higher.setSelected(false);
	       	    position_l_higher.setSelected(false);
	       	    position_equal.setSelected(false);
	       	    position_noRel.setSelected(false);
	       	    kindred_s_higher.setSelected(false);
	       	    kindred_l_higher.setSelected(false);
	       	    kindred_equal.setSelected(false);
	       	    kindred_noRel.setSelected(false);
	       	    s_much_older.setSelected(false);
	       	    s_older.setSelected(false);
	       	    equal.setSelected(false);
	       	    l_much_older.setSelected(false);
	       	    l_older.setSelected(false);
	       	    s_elder.setSelected(false);
	       	    s_adult.setSelected(false);
	       	    s_young_adult.setSelected(false);
	       	    s_teenager.setSelected(false);
	       	    s_child.setSelected(false);
	       	    l_elder.setSelected(false);
	       	    l_adult.setSelected(false);
	       	    l_young_adult.setSelected(false);
	       	    l_teenager.setSelected(false);
	       	    l_child.setSelected(false);

	       	   	ta.setText("Select all social factors above and click OK button!!!");
	       	   	ta.setForeground(Color.red);

	       	    f=""; k=""; r=""; d=""; s=""; l=""; i="";
	       	    f_count=0; k_count=0; r_count=0; d_count=0; s_count=0; l_count=0; i_count=0;
		   	}
		   	else if(event.getSource()==ok)
		   	{
				if(f_count!=1 || k_count!=1 || r_count!=1 || d_count!=1 ||
				   s_count!=1 || l_count!=1 || i_count!=1)
					{
				   		ta.setText("Select one per factor!!!");
						ta.setForeground(Color.red);

				   	}
				else
				{
					//ta.setText("OK");
					//ta.setForeground(Color.black);
					try
					{
						FileWriter fw = new FileWriter("selection.lp");
						BufferedWriter bw = new BufferedWriter (fw);
					    bw.write("taewon");
						bw.write("\nkim");
						bw.close();
					}
					catch(IOException e)
					{
						System.out.println("There was a problem: " + e);
					}
				}
     		}

		}
	}
}
