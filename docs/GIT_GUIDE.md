# How to use GitHub
This guide will show you how we will use
GitHub for our project. You guys probably
know how to do all of this stuff, but I'm
writing this for administrative purposes.
We will be using SSH keys for secure authentication
as well as branches so we don't mess up each other's code.

## Installing Git
You guys most likely have this installed
already but for the sake of completion,
I'll include it here. If you're on Linux,
you can install `git` using your distro's
package manager. If you're on macOS, it
should be installed if you have the Xcode
Command Line Tools. Run `xcode-select --install`
if you don't have those. If you're on Windows,
you will use the `MSYS MINGW` terminal throughout
the entire development process. Open the terminal
and run the following command to install `git` as
well as `openssh`
```bash
pacman -S git openssh
```

## SSH Keys
First thing we will do is setup SSH keys.
If you already use SSH keys to authenticate
with GitHub, you can skip this step. If not,
let's begin. First thing we will do is create
the `.ssh` directory. This is where we will
store our public and private keys. Run the
following commands to create the directory
as well as go into the directory.
```bash
mkdir ~/.ssh
cd ~/.ssh
```

Next we will make the SSH keys themselves.
Run the following command to make the
public and private keypair.
```bash
ssh-keygen -t ed25519 -C "your_email@example.com"
```

You will be prompted for the file name of the key.
You can call it whatever you want, but I suggest
calling it `github_key` for clarity. You will
then be prompted to enter an optional passphrase. Please
for the love of God put a passphrase on your SSH keys. In
the real world, SSH keys are valuable and if you're working
on classified software, and your keys get compromised,
the attacker can now push code as your account. Once you put
in the password, a public key and private key will be created.
The output should look something like this.
```text
Generating public/private ed25519 key pair.
Enter file in which to save the key (/home/briskycola/.ssh/id_ed25519): github_key
Enter passphrase for "github_key" (empty for no passphrase): 
Enter same passphrase again: 
Your identification has been saved in github_key
Your public key has been saved in github_key.pub
The key fingerprint is:
SHA256:xj+0oyXrQvL3gOUTypO61u7RMzM/9zg1+DitzVRWWCc briskycola@Briskycola-FWL
The key's randomart image is:
+--[ED25519 256]--+
|              E o|
|               +.|
|              . .|
|       .        .|
|        S .  .  o|
|    ...O + .. oo |
|     =B @ *  =.. |
|    ..+o.&.o==o  |
|   .o+o++.ooo=+  |
+----[SHA256]-----+
```
This is using asymmetric encryption. The private key
is used to cryptographically prove your identity
at the start of the connection. Then the GitHub SSH server
uses your public key to verify that you own the private key.
Finally, we will add our public key to GitHub. To do this,
log in to your GitHub account and go to **Settings** ->
**SSH and GPG keys** and click on **New SSH Key**. You
can set the title to whatever you want and leave Key Type
to Authentication Key. For the key itself, paste in
your public key exactly. To get the public key, run
the following:
```bash
cat ~/.ssh/github_key.pub
```

and you should see something that looks like this.
```text
ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIALar5Sok59dSVBgLFzyP4dDvq0u1tziNMgaGJiooY2E your_email@example.com
```

Note: This is not my actual public key, I just made a dummy
key for demo purposes. After this, GitHub with SSH authentication
should now work. To test it, go to any directory where you
store your repos and try to clone it.
```bash
git clone git@github.com:briskycola/nomp
```

You should be prompted to enter the passphrase for your
SSH key. Enter the passphrase and you should be good.
This is how we will get the code and contribute.

## Contributing
Everyone will be using their own branches to make
changes. If you feel confident about pushing to
main, you will make a pull request on GitHub.

### Creating your branch
To create your branch, go to the repo directory
and run the following commands:
```bash
git switch -c {your-branch-name}
git push -u origin {your-branch-name}
# Write your code
git add path/to/modified/file
git commit -m '{message describing your changes}'
git push origin {your-branch-name}
```

### Incrementally work inside your branch
This is the general rhythm of working inside
the branch. Throughout the development process,
you will be doing the following:
```bash
git switch main
git pull
git switch {your-branch-name}
git merge main
# If there are changes in main, run the following:
git push origin {your-branch-name}
# Write your code
git add path/to/modified/file
git commit -m '{message describing your changes}'
git push origin {your-branch-name}
```

### Pushing to main
Once you feel confident in your changes and you
want to push to main, make a pull request on
the GitHub website. To do this, go to the
repo website, then got to **Pull Requests**
-> **New Pull Request**. For `base:` make
it the main branch and for `compare:`, make
it your branch. Add in a title and description
and you should be good. We will review it
as a team and decide if it should be merged.
