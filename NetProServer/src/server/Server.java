package server;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

import server.model.Session;
import server.reciever.Receiver;
import server.sender.Sender;

public class Server {
	public static Receiver receiver = new Receiver();
	public static Sender sender = new Sender();
	public Map<SocketChannel, Session> sessionMap;

	public static void main(String[] args) {
		try {
			// Tạo một Selector
			Selector selector = Selector.open();

			// Tạo ServerSocketChannel và bind đến một cổng
			ServerSocketChannel serverSocketChannel = ServerSocketChannel.open();
			serverSocketChannel.socket().bind(new InetSocketAddress(5500));
			serverSocketChannel.configureBlocking(false);

			// Đăng ký ServerSocketChannel với Selector để lắng nghe kết nối đến
			serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT);

			System.out.println("Server is listening on port 5500...");

			while (true) {
				// Chờ các sự kiện có sẵn trong Selector
				selector.select();

				// Lấy tập hợp các SelectionKey
				Set<SelectionKey> selectedKeys = selector.selectedKeys();
				Iterator<SelectionKey> keyIterator = selectedKeys.iterator();

				while (keyIterator.hasNext()) {
					SelectionKey key = keyIterator.next();

					if (key.isAcceptable()) {
						// Sự kiện khi có kết nối mới
						ServerSocketChannel serverChannel = (ServerSocketChannel) key.channel();
						SocketChannel clientChannel = serverChannel.accept();
						clientChannel.configureBlocking(false);
						clientChannel.register(selector, SelectionKey.OP_READ);
						System.out.println("Accepted connection from: " + clientChannel.getRemoteAddress());
					} else if (key.isReadable()) {
						SocketChannel cliChannel = (SocketChannel) key.channel();
						receiver.receiveData(key);
						sender.sendMsg("Hello", cliChannel);
					}
					keyIterator.remove();
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
