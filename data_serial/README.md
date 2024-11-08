# serial usage
- https://stackoverflow.com/questions/52187/virtual-serial-port-for-linux

# socat setup
```
socat -d -d pty,raw,echo=0 pty,raw,echo=0
```

produces:
```
danny@abs:~$ socat -d -d pty,raw,echo=0 pty,raw,echo=
2024/11/08 08:12:13 socat[14606] N PTY is /dev/pts/12
2024/11/08 08:12:13 socat[14606] N PTY is /dev/pts/13
2024/11/08 08:12:13 socat[14606] N starting data transfer loop with FDs [5,5] and [7,7]
```

### read in terminal
```
cat < /dev/pts/12
```

### write in terminal
```
echo "Test" > /dev/pts/13
```