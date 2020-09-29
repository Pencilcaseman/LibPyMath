import math
from collections.abc import Iterable
import time
import shutil

class CharSet:
	def __init__(self, fill="█", blank=" ", half="▍"):
		self.fill = fill
		self.blank = blank
		self.half = half

	def eval(self, fill, length):
		fillChars = math.floor(length * fill)

		rem = (length * fill) % 1
		useHalf = rem > 0.25 and rem < 0.75

		res = self.fill * (fillChars + (1 if rem > 0.75 else 0)) + (self.half if useHalf else "")

		res += self.blank * (length - len(res))

		return res

class Progress:
	def __init__(self, iter=None, start=None, end=None, step=None, charset=None):
		self.iter = None
		self.start = None
		self.end = None
		self.step = None
		self.len = None
		self.charset = None

		self.count = 0
		self.maxIterLen = None

		# Start of __iter__
		self.startTime = 0

		# Iterations passed since previous update of bar
		self.iterationsPassed = 0

		# Time of the last update to the bar
		self.lastBarUpdate = 0

		if iter is None:
			# Use start, end and step
			if start is None:
				start = 0

			if end is None:
				raise ValueError("Either iter or end must be specified")

			if iter is None:
				iter = 1

			if not isinstance(start, (int, float)):
				raise TypeError("Start must be of type int or float")

			if not isinstance(end, (int, float)):
				raise TypeError("End must be of type int or float")

			if not isinstance(step, (int, float)):
				raise TypeError("Step must be of type int or float")

			if step == 0:
				raise ValueError("Step cannot be 0")

			if (end - start > 0 and step < 0) or (end - start < 0 and step > 0):
				raise Warning("Infinite loop created")

			self.start = start
			self.end = end
			self.step = step
			self.len = math.floor((end - start) / step)

		else:
			if not isinstance(iter, Iterable):
				raise TypeError("Iter must be an iterable object")

			self.iter = iter
			self.len = len(iter)

		if charset is None:
			self.charset = CharSet()
		elif isinstance(charset, CharSet):
			self.charset = charset

	def __iter__(self):
		iterable = self.iter
		start = time.time()
		its = 0
		count = 0
		iterationsPassed = 0
		previousIteration = 0
		maxLen = len(str(self.len))
		minutes = 0

		seconds = 0
		minutes = 0
		currentMinutes = 0
		currentSeconds = 0
		
		seconds = 0
		minutes = 0
		remainingMinutes = 0
		remainingSeconds = 0

		secondsPerIteration = False

		mod = 1
		timeMod = 1

		termWidth = shutil.get_terminal_size(fallback=(120, 50)).columns
		barWidth = 10

		previousYield = time.time()





		# Create the progress bar >>>  50%|##########__________| 50/100 [00:05|<|00:05  5 it/s]
		# Percentage
		prog = "{:>3}%".format(0)

		# Time
		# Calculate the iterations per second
		dt = 1
		secondsPerIteration = dt > 1
		its = 0

		seconds = 0
		minutes = 0
		currentMinutes = ("0" if len(str(minutes)) < 2 else "") + str(minutes)
		currentSeconds = ("0" if len(str(seconds)) < 2 else "") + str(seconds)

		dt = 1
		seconds = 0
		minutes = 0
		remainingMinutes = ("0" if len(str(minutes)) < 2 else "") + str(minutes)
		remainingSeconds = ("0" if len(str(seconds)) < 2 else "") + str(seconds)

		timeSection = " [{}:{}|<|{}:{}, {:.2f} {}]".format(currentMinutes, currentSeconds, remainingMinutes, remainingSeconds, its, "it/s" if not secondsPerIteration else "s/it")

		# 			Total width 	- 	percentage 	- Bar ends	- Progress 			- Time 				- Buffer
		barWidth = 	termWidth 		- 	4			- 2			- (2 + maxLen * 2)	- len(timeSection) 	- 1

		# Bar
		# prog += "|{}|".format(self.charset.eval(count / self.len, 100))
		prog += "|{}|".format(self.charset.eval(count / self.len, barWidth))

		# X/Y
		prog += " {}/{}".format(str(count).rjust(maxLen), self.len)

		prog += timeSection
		print(prog, end="\r")



		for ob in iterable:
			yield ob

			# The current time
			current = time.time()

			# print(max(current - start, 0.0000001))
			delta = 1 / max(current - previousYield, 0.0000001)

			previousYield = current

			if delta < 25:
				mod = 1
				timeMod = 1
			elif delta < 1000:
				mod = 1
				timeMod = 50
			elif delta < 10000:
				mod = 10
				timeMod = 2000
			elif delta < 100000:
				mod = 100
				timeMod = 100000
			elif delta < 1000000:
				mod = 1000
				timeMod = 100000000
			elif delta < 10000000:
				mod = 10000
				timeMod = 1000000000
			elif delta < 100000000:
				mod = 100000
				timeMod = 10000000000
			elif delta < 1000000000:
				mod = 1000000
				timeMod = 100000000000
			else:
				mod = 10000000
				timeMod = 10000000000000

			count += 1
			iterationsPassed += 1

			if count % mod == 0:
				# The current time
				current = time.time()

				try:
					# Create the progress bar >>>  50%|##########__________| 50/100 [00:05|<|00:05  5 it/s]
					# Percentage
					prog = "{:>3}%".format(round((count / self.len) * 100))

					# Time
					if count % timeMod == 0:
						# Calculate the iterations per second
						dt = (current - previousIteration) if count != 1 else 1
						secondsPerIteration = dt > 1
						its = (iterationsPassed / dt) if not secondsPerIteration else (dt / iterationsPassed)

						dt = round(current - start)
						seconds = dt % 60
						minutes = (dt // 60) % 60
						currentMinutes = ("0" if len(str(minutes)) < 2 else "") + str(minutes)
						currentSeconds = ("0" if len(str(seconds)) < 2 else "") + str(seconds)

						dt = int((self.len - count) / its)
						seconds = dt % 60
						minutes = (dt // 60) % 60
						remainingMinutes = ("0" if len(str(minutes)) < 2 else "") + str(minutes)
						remainingSeconds = ("0" if len(str(seconds)) < 2 else "") + str(seconds)

					timeSection = " [{}:{}|<|{}:{}, {:.2f} {}]".format(currentMinutes, currentSeconds, remainingMinutes, remainingSeconds, its, "it/s" if not secondsPerIteration else "s/it")

					# 			Total width 	- 	percentage 	- Bar ends	- Progress 			- Time 				- Buffer
					barWidth = 	termWidth 		- 	4			- 2			- (2 + maxLen * 2)	- len(timeSection) 	- 1

					# Bar
					# prog += "|{}|".format(self.charset.eval(count / self.len, 100))
					prog += "|{}|".format(self.charset.eval(count / self.len, barWidth))

					# X/Y
					prog += " {}/{}".format(str(count).rjust(maxLen), self.len)

					prog += timeSection
				except ZeroDivisionError:
					pass

				# Print the progress bar
				print(prog, end="\r")

				previousIteration = time.time()
				iterationsPassed = 0

		print()
		return
