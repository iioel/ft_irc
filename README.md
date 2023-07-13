# ft_irc

## How to run irssi

You can build an irssi docker image like this :

```bash
echo -e "FROM debian:bookworm-slim\nRUN apt update && apt install -y irssi ca-certificates\nENTRYPOINT /usr/bin/irssi" | docker build -t irssi -f - .
```

And you can launch it with this command :

```bash
docker run -it --rm --network host --add-host=host:host-gateway irssi
```

To make things simpler, you can add theses lines to your .bashrc/.zshrc :

```bash
alias irssi-build='echo -e "FROM debian:bookworm-slim\nRUN apt update && apt install -y irssi ca-certificates\nENTRYPOINT /usr/bin/irssi" | docker build -t irssi -f - .'
alias irssi='docker run -it --rm --network host --add-host=host:host-gateway irssi'
```

So this way you just need to build with `irssi-build` and launch it with `irssi`.

## Test the v1

Everything was hardcoded in the `main.cpp` file, so you need a specific environment to test it.

Oh, and it also support only *one* client.

### Recommendations

Start irssi in a docker with the commands above.

So once you are in irssi your `real_name`, `user_name` and `nick` should be root.

Start your `ft_irc` server on your host.

Next type `/connect host` on your irssi client, you will see some reply from the server.

To start sending messages, first join the chan `#chan` (yup, the name is hardcoded...) by typing `/join chan` in you irssi client.

You can now sent some messages ! Congratulations !!!
