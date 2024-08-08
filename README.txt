Project: Real Talk (rt)

Author: Daniel Gilbert

License:

- Please copy code from this project
  so you can learn from it
  and build on it.

- Leverage the design documents
  to feed your own ideas
  to make this project great.

Pupose of Project:

- To provide a secure means of
  peer-to-peer, end-to-end encrypted
  messaging
  with zero intermediary servers,
  relying solely on the existing network infrastructure.

===

Below is a snippet from the Design Log file that illustrates the desired
functionality of how this may be used once it is built.

For simplicity, and to start,
only support one server running on a host
at a time.

host (server):  > ./rt
                > Please enter a password that
                > will be required to join this chat:
                > (types password then presses enter)

                (rt starts server listening on 0.0.0.0 by default
                to simplify for non-technical users, and even
                automatically finds a port number to use that is not taken.
                The password is required for people to connect, but is only
                the first line of defense. The project will later incorporate
                GPG for every user so that all messages are encrypted
                and decrypted by automatically using the relevant key pairs.
                In addition, everyone will see if an unknown user joins,
                so they can stop talking if someone has broken in.
                New users will not see chats prior to when they joined.)

                > Server has been started.
                > The password is: (password)

                > All users will be prompted for the password upon connecting.

                > They have 3 password attempts.
                > If they fail the first 3, they will get disconnected
                > and you will be notified.
                > They can reconnect a second time and enter 3 more passwords,
                > but if those fail, they will be automatically blocked,
                > and you will have to unblock them. You will be notified.

                (Include geolocation/IP ownership info in notifications
                if I can pull that info without relying too much on a
                third-party or sending unwanted telemetry to those
                services. Maybe keep a cached copy of all the relevant
                geo/IP info so that queries aren't constantly being made,
                and the user can manually update the cache only if they desire.)

                (I'm trying to make this program as simple as possible to use
                for non-technical people.)

                > For people on the same computer network (WiFi, cable, etc.),
                > if they want to join, have them type
                > ./rt X.X.X.X XXXXX

                (If the user has multiple local IPs, list them for the user
                with instructions.)

                > For people outside of your computer network
                > (remote locations on the Internet),
                > if they want to join, have them type
                > ./rt Y.Y.Y.Y XXXXX (User's public IP is shown.)
                
                (goes to background)

                (joins server they just created)

client:         > ./rt remote_public_ip port

                (connects to server via NAT bypass or port forwarding, etc.)

                > Password: (types password, then presses enter)

                "Hey, what's up?"

host (server):  "Alright, talk to you later."

                (ctrl+c, disconnects self and all connected clients)
