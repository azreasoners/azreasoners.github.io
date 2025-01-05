// Project.java

import java.io.*;

public class Project
{
    public static void main(String[] args) throws IOException
    {
      //Runtime r = Runtime.getRuntime();
      
      // 1. Write an intermediate file called factors.lp
	String[] cmd={"/bin/sh", "-c", "lparse test.lp test2.lp | smodels 0"};
	Process p = Runtime.getRuntime().exec(cmd);
	String s=null;
	BufferedReader stdInput = new BufferedReader
	    (new InputStreamReader(p.getInputStream()));
	while( (s=stdInput.readLine()) !=null)
	    {
		System.out.println(s); 
	    }
	
      // 2. Select speech levels
      // 3. Show on the screen (GUI)

	System.out.println("END");
    }
}
