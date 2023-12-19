defmodule Solution do
  def part1(c) do
    c |> String.split("\n", trim: true)
      |> IO.inspect()
  end
  def part2(c) do
    c |> String.split("\n", trim: true)
      |> IO.inspect()
  end
end

# case File.read("puzzle.txt") do
case File.read("example.txt") do
  {:ok, content} ->
    Solution.part1(content)
    Solution.part2(content)
  {:error} -> IO.put("Failed to read example.txt")
end
