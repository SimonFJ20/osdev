
# bingbong

```
docker build . -t bingbong-buildenv

docker run --rm -it -v $PWD:/root/env bingbong-buildenv

qemu-system-x86_64 -cdrom dist/kernel.iso
```
