-- phpMyAdmin SQL Dump
-- version 4.6.5.2
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: 2017-03-16 14:33:40
-- 服务器版本： 10.1.21-MariaDB
-- PHP Version: 7.1.1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `msgBoard`
--

-- --------------------------------------------------------

--
-- 表的结构 `message`
--

CREATE TABLE `message` (
  `msgid` int(11) NOT NULL,
  `userid` int(11) DEFAULT NULL,
  `tm` datetime DEFAULT NULL,
  `context` text,
  `public` tinyint(1) DEFAULT NULL,
  `target` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

--
-- 转存表中的数据 `message`
--

INSERT INTO `message` (`msgid`, `userid`, `tm`, `context`, `public`, `target`) VALUES
(1, 1, '2017-03-14 00:00:00', '这条留言超长这条留言超长这条留言超长这条留言超长这条留言超长这条留言超长这条留言超长这条留言超长这条留言超长这条留言超长', 1, 0),
(2, 2, '2017-03-14 05:00:00', 'asdfasdgsfrabgvfrfrvg', 1, 0),
(3, 1, '2017-03-14 16:54:14', 'aaa', 1, 0),
(4, 2, '2017-03-14 17:30:03', 'hello world', 1, 0),
(32, 1, '2017-03-14 18:23:38', 'balabala', 1, 0),
(33, 1, '2017-03-14 18:23:55', 'hahaha', 1, 0),
(34, 1, '2017-03-15 15:25:46', '测试中文', 1, 0),
(35, 2, '2017-03-15 21:30:05', 'there is a very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very long very longvv', 1, 0),
(36, 4, '2017-03-16 17:39:08', 'æˆ‘æ˜¯æ¯”åˆ©', 1, 0),
(37, 4, '2017-03-16 17:42:13', '比利比利', 1, 0),
(38, 4, '2017-03-16 17:44:03', '比利\r\n', 1, 0);

-- --------------------------------------------------------

--
-- 表的结构 `userinfo`
--

CREATE TABLE `userinfo` (
  `userid` int(11) NOT NULL,
  `username` varchar(128) DEFAULT NULL,
  `password` varchar(128) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

--
-- 转存表中的数据 `userinfo`
--

INSERT INTO `userinfo` (`userid`, `username`, `password`) VALUES
(1, 'admin', 'admin'),
(2, 'user1', 'password'),
(3, 'aa', 'aa'),
(4, '比利', 'password');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `message`
--
ALTER TABLE `message`
  ADD PRIMARY KEY (`msgid`);

--
-- Indexes for table `userinfo`
--
ALTER TABLE `userinfo`
  ADD PRIMARY KEY (`userid`);

--
-- 在导出的表使用AUTO_INCREMENT
--

--
-- 使用表AUTO_INCREMENT `message`
--
ALTER TABLE `message`
  MODIFY `msgid` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=39;
--
-- 使用表AUTO_INCREMENT `userinfo`
--
ALTER TABLE `userinfo`
  MODIFY `userid` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
