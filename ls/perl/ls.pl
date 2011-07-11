#!/usr/bin/env perl 

use 5.010;
use utf8;
use strict;
use warnings;
use Getopt::Long qw(:config bundling);

use Const::Fast;
use DateTime;
use Encode qw(encode_utf8);
use File::Basename;
use File::Slurp;
use File::Spec::Functions;
use File::stat;

const my $MODE_FILE => 1;
const my $MODE_DIR  => 0;

my $long = 0;
my $all  = 0;

my $result = GetOptions(
    'l' => \$long,
    'a' => \$all,
);

my @dirs;
my @files;
(-d) ? push @dirs, $_ : push @files, $_ for sort @ARGV;
@dirs = ('.') unless @ARGV;

ls_file($MODE_FILE, @files);
print "\n" if @files && @dirs;

if (@dirs > 1) {
    for (my $i = 0; $i < @dirs; ++$i) {
        say "$dirs[$i]:";
        ls_dir($dirs[$i]);
        print "\n" unless $i == $#dirs;
    }
}
else {
    say "$dirs[0]:" if @files;
    ls_dir($dirs[0]);
}

sub get_mode {
    my $mode = shift;

    my $u = ($mode & 00700) >> 6;
    my $g = ($mode & 00070) >> 3;
    my $o = ($mode & 00007);

    return sprintf(
        '%s' x 9,

        ($u & 4) ? 'r' : '-',
        ($u & 2) ? 'w' : '-',
        ($u & 1) ? 'x' : '-',

        ($g & 4) ? 'r' : '-',
        ($g & 2) ? 'w' : '-',
        ($g & 1) ? 'x' : '-',

        ($o & 4) ? 'r' : '-',
        ($o & 2) ? 'w' : '-',
        ($o & 1) ? 'x' : '-',
    );
}

sub ls_file {
    my $mode  = shift;
    my @files = @_;

    return unless @files;

    my $max_link = 0;
    my $max_size = 0;
    for my $file ( @files ) {
        my $st = lstat($file);
        $max_link = $st->nlink if $st->nlink > $max_link;
        $max_size = $st->size  if $st->size  > $max_size;
    }

    my $link_len = length($max_link);
    my $size_len = length($max_size);

    for my $file ( sort @files ) {
        my $printed_file = ($mode == $MODE_DIR) ? basename($file) : $file;
        if ($long) {
            my $st = lstat($file);

            chomp( my $tz = read_file(catfile('/', 'etc', 'timezone')) );
            my $dt = DateTime->from_epoch(
                epoch     => $st->mtime,
                time_zone => $tz,
            );

            my $type = (-l $file) ? 'l'
                     : (-d $file) ? 'd'
                     :              '-'
                     ;

            if ($type eq 'l') {
                $printed_file .= ' -> ' . readlink $file;
            }

            printf(
                "%s%9s %${link_len}d %s %s %${size_len}d %s %02d:%02d %s\n",
                $type,
                get_mode($st->mode),
                $st->nlink,
                (getpwuid($st->uid))[0],
                (getgrgid($st->gid))[0],
                $st->size,
                $dt->ymd,
                $dt->hour,
                $dt->minute,
                $printed_file,
            );
        }
        else {
            say $printed_file;
        }
    }
}

sub ls_dir {
    my $dir = shift;

    my @files;
    if ($all) {
        @files = read_dir( $dir, keep_dot_dot => 1 );
    }
    else {
        @files = grep { /^[^.]/ } read_dir($dir);
    }

    my $blk_size = 0;
    for my $file ( @files ) {
        my $st = lstat( catfile($dir, $file) );
        $blk_size
            += (int($st->size / 4096) + ($st->size % 4096 ? 1 : 0))
            * (4096 / 1024)
            ;
    }
    say encode_utf8("합계 $blk_size") if $long;

    ls_file( $MODE_DIR, map catfile($dir, $_), @files );
}
