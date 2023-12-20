package client.sender;

import java.io.IOException;
import java.io.OutputStream;

public class Sender {
	public void send(String msg, OutputStream output) {
		try {
			byte[] msgBytes = msg.getBytes("UTF-8");
			output.write(msgBytes);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
