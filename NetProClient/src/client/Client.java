package client;

import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;

import client.receiver.Receiver;
import client.sender.Sender;

public class Client {

    public static void main(String[] args) {
        final String serverAddress = "127.0.0.1";
        final int serverPort = 5500; 
        Sender sender = new Sender();
        Receiver receiver = new Receiver();

        try (Socket socket = new Socket(serverAddress, serverPort)) {
            OutputStream outputStream = socket.getOutputStream();
            sender.send("Hay vcl", outputStream);
            

            receiver.receiveMsg(new InputStreamReader(socket.getInputStream()));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}


