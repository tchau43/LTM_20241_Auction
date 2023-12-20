package server.reciever;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;

public class Receiver {
	public int receiveData(SelectionKey cliKey) throws IOException {
		SocketChannel cliChannel = (SocketChannel) cliKey.channel();
		ByteBuffer buffer = ByteBuffer.allocate(1024);
		int bytesRead;

		try {
			bytesRead = cliChannel.read(buffer);
		} catch (IOException e) {
			System.out.println("Client disconnected abruptly");
			cliKey.cancel();
			cliChannel.close();
			return -1;
		}

		if (bytesRead > 0) {
			buffer.flip();
			byte[] data = new byte[bytesRead];
			buffer.get(data);
			String message = new String(data);
			System.out.println("Received message: " + message);

		} else {
			cliChannel.close();
		}

		return 0;
	}

}
