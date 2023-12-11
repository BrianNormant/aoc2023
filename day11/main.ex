defmodule Solution do
  def part1(c) do
  end
  def part2(c) do
  end
end

case File.read("example.txt") do
  {:ok, content} ->
    part1(content)
  {:error} -> IO.put("Failed to read example.txt")
end
