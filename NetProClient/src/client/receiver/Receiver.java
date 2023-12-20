package client.receiver;

import java.io.BufferedReader;
import java.io.InputStreamReader;

public class Receiver {
	public void receiveMsg(InputStreamReader inputstream) {
		try {
		BufferedReader reader = new BufferedReader(inputstream);
		String response = reader.readLine();
		System.out.println("Server response: " + response);
		} catch(Exception e){
			e.printStackTrace();
		}
	}
}
