package server.sender;

import java.io.IOException;
import java.io.OutputStream;
import java.nio.channels.SocketChannel;
import java.nio.charset.StandardCharsets;

public class Sender{
	public void sendMsg(String msg, SocketChannel socket) {
		try {
			OutputStream output = socket.socket().getOutputStream();
			byte[] msgBytes = msg.getBytes(StandardCharsets.UTF_8);
			output.write(msgBytes);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
}