defmodule Solution do

  def transpose(m) do
    Enum.reduce(0..((Enum.at(m,0) |> Enum.count())-1),[], fn (j, acc) ->
      acc ++ [Enum.reduce(0..(Enum.count(m)-1), [], fn (i, acc) ->
          acc ++ [Enum.at(m, i) |> Enum.at(j)]
      end)]
    end)
  end

  def parse(grid) do
    lines = grid 
      |> Enum.map( &Enum.all?(&1, fn s -> s == "." end) )
      |> Enum.map( &if &1 do 1 else 0 end)
      
    columns = grid
      |> transpose()
      |> Enum.map( &Enum.all?(&1, fn s -> s == "." end) )
      |> Enum.map( &if &1 do 1 else 0 end)
    
    grid = grid
      |> Enum.map( &Enum.with_index(&1) )
      |> Enum.with_index()
    %{:grid => grid, :lines => lines, :columns => columns}
  end
  
  def find_stars(all) do
  %{lines: lines, columns: columns} = all
  Map.update!( all, :grid, fn g ->
        g |>
          Enum.map(fn {l, y} ->
            { Enum.filter(l, fn {c, _} -> c == "#" end), y}
          end)
          |> Enum.filter(fn {l, _} -> l != [] end)
          |> Enum.map(fn {l, y} -> 
            deply = if y != 0 do
            Enum.slice(lines, 0, y)
              |> Enum.sum()
            else 0 end
            Enum.map(l, fn {_, x} ->
              deplx = if x != 0 do
                Enum.slice(columns, 0, x)
                  |> Enum.sum()
                else 0 end
                            
              {
                x - deplx + deplx * 100,
                y - deply + deply * 100
              }
            end)
          end)
          |> List.flatten()
    end)
  end

  def pairs([a | rest]) do # [e, e1, e2, e3] -> [[e, e1], [e, e2], ...]
    rest |> Enum.map( &( {a, &1} ) )
  end

  def continuous_apply([], fun), do: []
  
  def continuous_apply(l, fun) do
    [_ | r] = l
    [fun.(l)] ++ continuous_apply(r, fun)
  end

  def distances(all) do
    %{grid: stars, lines: lines, columns: columns} = all
    stars
      |> continuous_apply(&pairs/1)
      |> Enum.drop(-1)
      |> Enum.map( fn pairs ->
          pairs
            |>Enum.map( fn {{x1, y1}, {x2, y2}} ->
              abs(x1 - x2) + abs(y1 - y2)
            end)
      end)
  end

  def part1(c) do
  c |> String.split("\n", trim: true)
    |> Enum.map(&String.graphemes/1)
    |> parse()
    |> find_stars()
    |> distances()
    |> List.flatten()
    |> Enum.sum()
    |> IO.inspect()
  end

  def part2(c) do
  end
end

case File.read("example.txt") do
  {:ok, content} ->
    Solution.part1(content)
  {:error} -> IO.put("Failed to read example.txt")
end
